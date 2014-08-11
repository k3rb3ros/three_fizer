#include "include/cbc.h"

void cbcDecryptInPlace(int32_t block_size, uint8_t* key, uint64_t* cipher_text, uint64_t size)
{
    //if(password != NULL && size % block_size == 0) //this should be moved outside cbc
    {
        //uint8_t* key = hash(password, block_size/8, block_size);

	//free(key);
    }
}

void cbcEncryptInPlace(int32_t block_size, uint8_t* key, uint64_t* plain_text, uint64_t size)
{
    //if(password != NULL && size % block_size == 0) 
    {
        uint8_t* initialization_vector = getRand(block_size/8);
        //uint8_t* key = hash(password, block_size/8, block_size); //this shoudl be moved outside cbc

        printf("Key: ");
        for(uint16_t i=0; i<block_size/8; ++i)
        {
            printf("%x ", key[i]);
        }
        printf("\n\nIV: ");
        for(uint16_t k=0; k<block_size/8; ++k)
        {
            printf("%x ", initialization_vector[k]);
        }
        free(initialization_vector);
        free(key);
    }
}
