#include "include/key.h"

bool handleKeys(arguments* args,
                ThreefishKey_t* cipher_context,
                MacCtx_t* mac_context)
{
    pdebug("handleKeys()\n");
    //sanity check
    if(args == NULL || cipher_context == NULL || mac_context == NULL)
    { return false; }

    const uint64_t block_byte_size = (uint64_t)args->state_size/8;
    uint64_t* cipher_key = NULL;
    uint64_t* mac_key = NULL;

    //no hash password (bad idea)
    if(args->hash == false && args->hash_from_file == false)
    {
        cipher_key = noHashKey(args->password, args->pw_length, args->state_size);
    } //no hash from file (bad idea)
    else if(args->hash == false && args->hash_from_file == true)
    {
	    cipher_key = noHashBlockFromFile(args->key_file, args->state_size);
    } //hash user entered password into key
    else if(args->hash == true && args->hash_from_file == false)
    {
        cipher_key = (uint64_t*)keyHash(args->password,
                                        args->pw_length,
                                        args->state_size);
    } //hash user entered password from file
    else if(args->hash == true && args->hash_from_file == true)
    {
        cipher_key = hashKeyFromFile(args->key_file, args->state_size);
    }
    
    if(cipher_key == NULL) { return false; } //failure

    //Generate IV if we are encrypting
    if(args->encrypt == true)
    {
        if(genIV(args) == false) { return false; } //can't continue without an IV
    }
    else //and get it from the first part of the file if we are decrypting
    {
        if(getIV(args) == false) { return false; }
    }

    //stretch the key with scrypt
    if(args->hash == true && args->legacy_hash == false)
    {
        printf("Stretching key this may take a bit.\n");
        if(kdf_scrypt((uint8_t*)cipher_key, (size_t)(args->state_size/8),
                       (uint8_t*)args->iv, (size_t)(args->state_size/8),
                       SCRYPT_N, SCRYPT_R,
                       SCRYPT_P, (uint8_t*)cipher_key,
                       (size_t)(args->state_size/8)
                      
                     ) != 0
          )
          { return false; } //scrypt failed
        printf("Done\n");
    }

    //generate the mac key from the cipher_key
    mac_key = (uint64_t*)keyHash((uint8_t*)cipher_key,
                                 block_byte_size,
                                 args->state_size);

    //initialize the key structure for the cipher key
    threefishSetKey(cipher_context,
                    (ThreefishSize_t)args->state_size, 
                    cipher_key,
                    threefizer_tweak);
    //initialize the mac context and undelying skein structures
    InitMacCtx(args, mac_context, mac_key);

    //free allocated resources
    if(cipher_key != NULL) { free(cipher_key); }
    if(mac_key != NULL) { free(mac_key); }

    return true;
}
