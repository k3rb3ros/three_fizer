#include "include/cbc.h"

void cbcDecryptInPlace(SkeinSize_t skein_size, uint8_t* key, uint64_t* cipher_text, uint64_t size)
{
    if(key != NULL && size % skein_size == 0) //this should be moved outside cbc
    {

    }
}

void cbcEncryptInPlace(SkeinSize_t skein_size, uint8_t* key, uint64_t* plain_text, uint64_t size)
{
    if(key != NULL && size % skein_size == 0) 
    {
        uint8_t* initialization_vector = getRand(skein_size/8);

        printf("Key: ");
        for(uint16_t i=0; i<skein_size/8; ++i)
        {
            printf("%x ", key[i]);
        }
        printf("\n\nIV: ");
        for(uint16_t k=0; k<skein_size/8; ++k)
        {
            printf("%x ", initialization_vector[k]);
        }
        free(initialization_vector);
        free(key);
    }
}
