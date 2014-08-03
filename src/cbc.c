#include "include/cbc.h"
#include "include/hash.h"
#include "include/random.h"

void cbc_decrypt(int32_t block_size, uint8_t* password, uint64_t* cipher_text, uint64_t size)
{
    uint8_t* key = hash(password, block_size/8, block_size);

	free(key);
}

void cbc_encrypt(int32_t block_size, uint8_t* password, uint64_t* plain_text, uint64_t size)
{
    uint8_t* initialization_vector = getRand(block_size/8);
    uint8_t* key = hash(password, block_size/8, block_size);

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

void cbc_teardown()
{
    
}

void init_state()
{
    
}

