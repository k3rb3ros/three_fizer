#include "cipher.h"

cipher initCipher(uint8_t* filename, uint8_t* password, SkeinSize_t skein_size)
{
    cipher cipher_internal;

    return cipher_internal;
}

int run_cipher(arguments* args, char* filename)
{
   int status = 0;
   if(args->encrypt)
   {
       encrypt(filename, args->password, args->pw_length, args->skein_size);
   }
   else
   {
       decrypt(filename, args->password, args->pw_length, args->skein_size);
   }
   return status;
}

bool decrypt(uint8_t* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t skein_size)
{
    uint8_t* key = hash(password, skein_size/8, skein_size);
    uint64_t* cipher_text = readFile(filename);
    uint64_t length = getSize(filename);

    cbcDecryptInPlace(skein_size, key, cipher_text, length); 
    if(key != NULL) free(key);
    if(cipher_text != NULL) free(cipher_text);
}

bool encrypt(uint8_t* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t skein_size)
{
    uint8_t* key = hash(password, skein_size/8, skein_size);
    //uint64_t* plain_text = readFile(filename);
    //uint64_t length = getSize(filename);

    //cbcEncryptInPlace(skein_size, key, plain_text, length);
    if(key != NULL) free(key);
    //if(plain_text != NULL) free(plain_text);
}
