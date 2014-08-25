#include "cipher.h"

cipher initCipher(uint8_t* filename, uint8_t* password, SkeinSize_t state_size)
{
    cipher cipher_internal;

    return cipher_internal;
}

int run_cipher(arguments* args, char* filename)
{
   int status = 0;
   if(args->encrypt)
   {
       encrypt(filename, args->password, args->pw_length, args->state_size);
   }
   else
   {
       decrypt(filename, args->password, args->pw_length, args->state_size);
   }
   return status;
}

bool decrypt(uint8_t* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t state_size)
{
    uint8_t* key = sf_hash(password, pw_length, state_size/8, state_size);
    uint64_t length = getSize(filename);
    uint64_t num_blocks = getNumBlocks(length, state_size);
    uint64_t* cipher_text = pad(readFile(filename), getSize(filename), state_size);

    cbcDecryptInPlace(state_size, key, cipher_text, num_blocks); 
    //Write decrypted text to file
    if(key != NULL) free(key);
    if(cipher_text != NULL) free(cipher_text);
}

bool encrypt(uint8_t* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t state_size)
{
    uint8_t* key = sf_hash(password, pw_length, state_size/8, state_size);
    uint64_t length = getSize(filename);
    uint64_t num_blocks = getNumBlocks(length, state_size);
    uint64_t* plain_text = pad(readFile(filename), getSize(filename), state_size);
    
    cbcEncryptInPlace(state_size, key, plain_text, num_blocks); //encrypt the input
    writeFile(filename, (uint8_t*)plain_text, length); //write it back to the file

    if(key != NULL) free(key);
    if(plain_text != NULL) free(plain_text);
}
