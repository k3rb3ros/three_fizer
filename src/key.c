#include <gcrypt.h>

#include "include/key.h"
#include "include/tweak.h"          //cipher_tweak constant

bool handleKeys(const arguments* args,
                      ThreefishKey_t* cipher_context,
                      MacCtx_t* mac_context)
{
    pdebug("handleKeys()\n");
    if(cipher_context == NULL && mac_context == NULL) { return false; } //sanity check

    const uint64_t block_byte_size = (uint64_t)args->state_size/8;
    uint64_t* cipher_key = NULL;
    uint64_t* mac_key = NULL;

    void * phash;
    void * fhash;
    void * shash;
    void * ksalt;
    unsigned long iterations = 1;
    unsigned long N = 262144; // 2^18

    if(args->hash == true && args->hash_from_file == false)
    {
        //hash the user entered password so the key matches state size
        cipher_key = calloc(1, block_byte_size);
        phash = keyHash(args->password, args->pw_length, args->state_size);
        shash = keyHash(args->salt, args->st_length, args->state_size);

        printf("Deriving cipher key... ");
        if(gcry_kdf_derive (phash, block_byte_size,
                            GCRY_KDF_SCRYPT, N, shash, block_byte_size,
                            iterations, block_byte_size, cipher_key) != 0)
        {
            return false;
        }
        printf("done!\n");
    }
    else if(args->hash == true && args->hash_from_file == true)
    {
        //hash the key file to a key of state size
        fhash = hashKeyFromFile(args->key_file, args->state_size);
        ksalt = calloc(1, block_byte_size);
        printf("Deriving salt for keys... ");
        if(gcry_kdf_derive (fhash, block_byte_size,
                            GCRY_KDF_SCRYPT, 32768, fhash, block_byte_size,
                            iterations, block_byte_size, ksalt) != 0)
        {
            return false;
        }
        printf("done!\n");
        printf("Deriving cipher key... ");
        if(gcry_kdf_derive (fhash, block_byte_size,
                            GCRY_KDF_SCRYPT, N, ksalt, block_byte_size,
                            iterations, block_byte_size, cipher_key) != 0)
        {
            return false;
        }
        printf("done!\n");
    }
    else if(args->hash == false && args->hash_from_file == false)
    {
        //use the user entered password as the key directly
        cipher_key = noHashKey(args->password, args->pw_length, args->state_size);
    }
    else if(args->hash == false && args->hash_from_file == true)
    {
        //Use first block of bytes directly from file
    	printf("***Warning*** You have turned off password hashing and specified a password file.\n");
        printf("If the file you have specified is shorter then the key size it will be rejected.\n");
        printf("If it is greater then the keysize then all bits greater then the key size will be truncated.\n");
        printf("This poses a security risk (do not do this unless you know exactly what you are doing)\n");
    	cipher_key = noHashBlockFromFile(args->key_file, args->state_size);
    }
    
    if(cipher_key == NULL) { return false; }

    //generate the mac key from the cipher_key
    mac_key = calloc(1, block_byte_size);
    printf("Deriving MAC key... "); 
    if(gcry_kdf_derive (cipher_key, block_byte_size, 
                        GCRY_KDF_SCRYPT, N, shash, block_byte_size,
                        iterations, block_byte_size, mac_key) != 0)
    {
        return false;
    }
    printf("done!\n");

    //initialize the key structure for the cipher key
    threefishSetKey(cipher_context, (ThreefishSize_t)args->state_size, cipher_key, threefizer_tweak);
    //initialize the mac context and underlying skein structures
    InitMacCtx(args, mac_context, mac_key);

    //free allocated resources
    if(cipher_key != NULL) { free(cipher_key); }
    if(mac_key != NULL) { free(mac_key); }

    return true;
}