#include "cipher.h"

cipher initCipher(uint8_t* filename, uint8_t* password, SkeinSize_t skein_size)
{
    cipher cipher_internal;
    
    cipher_internal.file = openForBlockRead((const char*)filename); //this is going to differ between encrypting and decrypting
    cipher_internal.key = hash(password, strlen((char*)password), skein_size);
    cipher_internal.padded_input = 0; 

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

bool decrypt(uint8_t* filename, uint8_t* password, uint32_t pw_length, SkeinSize_t skein_size)
{
    uint8_t* key = NULL;
    uint64_t* plain_text = NULL;
    uint64_t length;
    cbcDecryptInPlace(skein_size, key, plain_text, length); 
}

bool encrypt(uint8_t* filename, uint8_t* password, uint32_t pw_length, SkeinSize_t skein_size)
{
    uint8_t* key = NULL;
    uint64_t* plain_text = NULL;
    uint64_t length;
    cbcEncryptInPlace(skein_size, key, plain_text, length);
}
