#include "include/key.h"

bool handleKeys(const arguments* args,
                ThreefishKey_t* cipher_context,
                MacCtx_t* mac_context)
{
    pdebug("handleKeys()\n");
    if(cipher_context == NULL && mac_context == NULL) { return false; } //sanity check

    const uint64_t block_byte_size = (uint64_t)args->state_size/8;
    uint64_t* cipher_key = NULL;
    uint64_t* mac_key = NULL;

    if(args->hash == true && args->hash_from_file == false)
    {
        //hash the user entered password so the key matches state size
        cipher_key = (uint64_t*)keyHash(args->password, args->pw_length, args->state_size);
    }
    else if(args->hash == true && args->hash_from_file == true)
    {
        //hash the key file to a key of state size
        cipher_key = hashKeyFromFile(args->key_file, args->state_size);
    }
    else if(args->hash == false && args->hash_from_file == false)
    {
        //use the user entered password as the key directly
        cipher_key = noHashKey(args->password, args->pw_length, args->state_size);
    }
    else if(args->hash == false && args->hash_from_file == true)
    {
        //Use first block of bytes directly from file
	    printf("***Warning*** You have turned off password hashing and specified a password file. If the file you have specified is shorter then the key size it will be rejected. If it is greater then the keysize then all bits greater then the key size will be truncated. This poses a security risk (do not do this unless you know exactly what you are doing)\n");
	    cipher_key = noHashBlockFromFile(args->key_file, args->state_size);
    }
    
    if(cipher_key == NULL) { return false; }

    //generate the mac key from the cipher_key
    mac_key = (uint64_t*)keyHash((uint8_t*)cipher_key, block_byte_size, args->state_size);

    //initialize the key structure for the cipher key
    threefishSetKey(cipher_context, (ThreefishSize_t)args->state_size, cipher_key, threefizer_tweak);
    //initialize the mac context and undelying skein structures
    InitMacCtx(args, mac_context, mac_key);

    //free allocated resources
    if(cipher_key != NULL) { free(cipher_key); }
    if(mac_key != NULL) { free(mac_key); }

    return true;
}
