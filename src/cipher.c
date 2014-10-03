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
    bool status = true;
    pdebug("decrypt()\n");
    const uint64_t block_size = (uint64_t)state_size/8;
    const uint64_t file_size = getSize(filename);

    if(file_size % block_size == 0)
    {
        FILE* fh = openForBlockRead(filename); //open the file for reading
        uint64_t* key = (uint64_t*)sf_hash(password, pw_length, block_size, state_size); //hash the user entered password so it is of state_size
        uint64_t* iv = (uint64_t*)readBlock(block_size, fh); //read the IV (first block)
        uint8_t* disp = iv;

        uint64_t* header = (uint64_t*)readBlock(block_size, fh);
        uint64_t* data = (uint64_t*)readBlock((file_size-(block_size*2)), fh); //read + buffer the cipher text
        fclose(fh); //close the file	

        uint64_t data_size = 0; 
        uint64_t tweak[2] = { 0L, 0L }; //allocate a tweak for the threefish key structure
        static ThreefishKey_t tf_key;
        threefishSetKey(&tf_key, (ThreefishSize_t)state_size, key, tweak); //set up the 3fish key structure

        if(check_header(&tf_key, iv, header, &data_size, state_size) && data_size > 0) //if the header is valid continue with decrypt
        {
            uint64_t num_blocks = getNumBlocks(data_size, state_size);
            pdebug("Data size:%lu num_blocks:%lu\n", data_size, num_blocks);
            DecryptInPlace(&tf_key, num_blocks, header, data, state_size); //decrypt the cipher text 
            pdebug("(Decrypt) Decrypted contents of file: [%s]\n", data);
	    fh = openForBlockWrite(filename); //open the file for writing
            writeBlock((uint8_t*)data, data_size-1, fh); //write the decrypted data to the file
            terminateFile(fh);
        }
	else
        {
            pdebug("Header size: %lu\n", data_size);
            status = false;
            perror("Header check failed either password is incorrect or file was not encrypted with this program\n");
        }
        
	//free all malloced resources
	//if(data != NULL) free(data);
        if(header != NULL) free(header);
        if(iv != NULL) free(iv); 
        if(key != NULL) free(key);
    }
    else
    {
        status = false;
        fprintf(stderr, "File is too small or not a multiple of %d bit block size\n", state_size);
    }

    return status;
}

bool check_header(ThreefishKey_t* key, uint64_t* iv, uint64_t* header, uint64_t* file_size, SkeinSize_t state_size)
{
    DecryptInPlace(key, 1, iv, header, state_size);

    if(header[0] == MAGIC_NUMBER && header[2] == state_size)//if the header check passes
    {
        *file_size = header[1]; //store the unpadded file size
        EncryptInPlace(key, 1, iv, header, state_size);//rencrypt the header as this is the iv for the next block
        return true;
    } 
    return false;
}

bool encrypt(const char* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t state_size)
{
    pdebug("encrypt()\n");
    const uint64_t block_size = ((uint64_t)state_size/8L);
    const uint64_t file_size = getSize(filename);

    if(file_size > 0)
    {
        uint64_t num_blocks = getNumBlocks(file_size, state_size); //Calculate the number of blocks from the file size
        uint64_t* data = pad(readFile(filename), getSize(filename), state_size); //zero pad the plain text
        uint64_t* key = (uint64_t*)sf_hash(password, pw_length, block_size, state_size); //hash the user entered password so it is of state_size
        uint64_t tweak[2] = { 0L, 0L }; //allocate a tweak for the threefish key structure
        static ThreefishKey_t tf_key;
        threefishSetKey(&tf_key, (ThreefishSize_t)state_size, key, tweak); //set up the 3fish key structure
        uint64_t* iv = (uint64_t*)getRand(block_size); //Get a block of random data for the IV
        uint64_t* header = gen_header(&tf_key, file_size, iv, state_size); //create the encrypted header block

        EncryptInPlace(&tf_key, num_blocks, header, data, state_size); //encrypt the data

        FILE* fh = openForBlockWrite(filename); //Open I/O for writing
        writeBlock((uint8_t*)iv, block_size, fh); //write the IV to the file
        writeBlock((uint8_t*)header, block_size, fh); //write the Header to the file
        writeBlock((uint8_t*)data, (block_size*num_blocks), fh);
        fclose(fh);       
 
        //free all dynamically allocated resources
        if(key != NULL) free(key);
        if(iv != NULL) free(iv);
        if(header != NULL) free(header);
        if(data != NULL) free(data);
        return true;
    }
    else
    {
        perror("File is empty aborting...\n");
        return false;
    }
}

uint64_t* gen_header(ThreefishKey_t* key, uint64_t data_size, uint64_t* iv, SkeinSize_t state_size) 
{
    uint64_t* header = NULL;

    if(iv != NULL && key != NULL && data_size > 1)
    {
        header = calloc(sizeof(uint64_t), state_size/64);
        header[0] = MAGIC_NUMBER;
        header[1] = data_size;
        header[2] = (uint64_t) state_size;
        header[3] = RESERVED;
       
        EncryptInPlace(key, 1, iv, header, state_size); //encrypt the header
    }

    return header;
}

void DecryptInPlace(ThreefishKey_t* key, uint64_t num_blocks, uint64_t* iv, uint64_t* cipher_text, SkeinSize_t state_size)
{
    switch(state_size) //call the corresponding decrypt and decrypt the header
    {
        case 256: cbc256Decrypt(key, iv, cipher_text, num_blocks);
        break;
        case 512: cbc512Decrypt(key, iv, cipher_text, num_blocks);
        break;
        case 1024: cbc1024Decrypt(key, iv, cipher_text, num_blocks);
        break;
        default:
        {
            perror("Invalid state size cannot continue\n");
            exit(8);
        }
        break;
    }
}

void EncryptInPlace(ThreefishKey_t* key, uint64_t num_blocks, uint64_t* iv, uint64_t* plain_text, SkeinSize_t state_size)
{
    switch(state_size) //call the corresponding decrypt and decrypt the header
    {
        case 256: cbc256Encrypt(key, iv, plain_text, num_blocks);
        break;
        case 512: cbc512Encrypt(key, iv, plain_text, num_blocks);
        break;
        case 1024: cbc1024Encrypt(key, iv, plain_text, num_blocks);
        break;
        default:
        {
            perror("Invalid state size cannot continue\n");
            exit(9);
        }
        break;
    }
}

