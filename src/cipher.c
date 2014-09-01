#include "cipher.h"

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

bool decrypt(const char* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t state_size)
{
    const uint64_t block_size = (uint64_t)state_size/8;
    const uint64_t file_size = getSize(filename);

    if((file_size >= block_size * 2) && (file_size % block_size == 0))
    {
        uint8_t* key = sf_hash(password, pw_length, block_size, state_size);
        uint64_t length = getSize(filename);
        uint64_t num_blocks = getNumBlocks(length, state_size);

        FILE* fh = openForBlockRead(filename); //open the file
        uint64_t* iv = (uint64_t*)readBlock(block_size, fh); //read the IV (first block)
        uint64_t* cipher_text = (uint64_t*)readBlock((file_size-block_size), fh); //read the cipher text

        cbcDecryptInPlace(state_size, iv, key, cipher_text, num_blocks); //decrypt the cipher text 
        //Write decrypted text to file
        if(key != NULL) free(key);
        if(cipher_text != NULL) free(cipher_text);
        return true;
    }
    else
    {
        fprintf(stderr, "File is too small or not a multiple of %d bit block size\n", state_size);
        return false;
    }
}

bool encrypt(const char* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t state_size)
{
    const uint64_t block_size = (uint64_t)state_size/8;
    const uint64_t file_size = getSize(filename);

    if(file_size > 0)
    {
        FILE* fh = openForBlockWrite(filename);
        uint8_t* key = sf_hash(password, pw_length, block_size, state_size);
        uint64_t* initialization_vector = (uint64_t*)getRand(block_size); //Fill our IV with Random or Pseudo random numbers
        uint64_t length = getSize(filename);
        uint64_t num_blocks = getNumBlocks(length, state_size);
        uint64_t* plain_text = pad(readFile(filename), getSize(filename)+1, state_size);
   
        writeBlock((uint8_t*)initialization_vector, block_size, fh); //write the iv to the file
        cbcEncryptInPlace(state_size, initialization_vector, key, plain_text, num_blocks); //encrypt the input
        writeBlock((uint8_t*)plain_text, block_size, fh); //write the encrypted data to the file
        fclose(fh);
        free(initialization_vector);
        if(key != NULL) free(key);
        if(plain_text != NULL) free(plain_text);
        return true;
    }
    else
    {
        perror("File is emmpty aborting...\n");
        return false;
    }
}
