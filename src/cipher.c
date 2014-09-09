#include "cipher.h"

int run_cipher(arguments* args, char* filename)
{
   int status = 0;

   if(args->encrypt)
   {
       if(encrypt(filename, args->password, args->pw_length, args->state_size))
       {
           printf("Succesfully encrypted file\n");
       }
   }
   else
   {
       if(decrypt(filename, args->password, args->pw_length, args->state_size))
       {
           pdebug("Succesfully decrypted file\n");
       }
   }
   return status;
}

bool decrypt(const char* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t state_size)
{
    pdebug("decrypt()\n");
    const uint64_t block_size = (uint64_t)state_size/8;
    const uint64_t file_size = getSize(filename);
    pdebug("(Decrypt) file size:%lu\n", file_size);

    if((file_size >= block_size * 2) && (file_size % block_size == 0))
    {
        uint8_t* key = sf_hash(password, pw_length, block_size, state_size);
        uint64_t num_blocks = getNumBlocks(file_size, state_size);
        pdebug("(Decrypt) num_blocks: %lu\n", num_blocks);

        FILE* fh = openForBlockRead(filename); //open the file for reading
        uint64_t* initialization_vector = (uint64_t*)readBlock(block_size, fh); //read the IV (first block)
        uint64_t* cipher_text = (uint64_t*)readBlock((file_size-block_size), fh); //read + buffer the cipher text
        fclose(fh); //close the file	
        pdebug("(Decrypt) contents of file: iv:[%s]data:[%s]\n", initialization_vector, cipher_text);

	fh = openForBlockWrite(filename); //open the file for writing
        cbcDecryptInPlace(state_size, initialization_vector, key, cipher_text, num_blocks-1); //decrypt the cipher text 
        pdebug("(Decrypt) Decrypted contents of file: [%s]\n", cipher_text);
        writeBlock((uint8_t*)cipher_text, (num_blocks*block_size), fh); //write the decrypted data to the file
        fclose(fh); //close the file
        if(initialization_vector != NULL) free(initialization_vector);
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
    pdebug("encrypt()\n");
    const uint64_t block_size = (uint64_t)state_size/8;
    const uint64_t file_size = getSize(filename);

    if(file_size > 0)
    {
        uint8_t* key = sf_hash(password, pw_length, block_size, state_size); //hash the pw so that it takes up the entire state_size of the cipher
        uint64_t* initialization_vector = (uint64_t*)getRand(block_size); //Get a block of random data for our IV
        uint64_t num_blocks = getNumBlocks(file_size, state_size); //Calculate the number of blocks from the file size
        uint64_t* plain_text = pad(readFile(filename), getSize(filename), state_size); //zero pad the plain text
        pdebug("file_size:%lu num_blocks:%lu\n", file_size, num_blocks);
        FILE* fh = openForBlockWrite(filename); //Open I/O for writing
        writeBlock((uint8_t*)initialization_vector, block_size, fh); //write the IV to the file
        pdebug("(Encrypt) text before encrypt: [%s]\n", plain_text);
        cbcEncryptInPlace(state_size, initialization_vector, key, plain_text, num_blocks); //encrypt the input
        pdebug("(Encrypt) text after encrypt: [%s]\n", plain_text);
        writeBlock((uint8_t*)plain_text, (num_blocks*block_size), fh); //write the encrypted data to the file
        fclose(fh); //close our file 
        //free all dynamically allocated resources
        if(initialization_vector != NULL) free(initialization_vector);
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
