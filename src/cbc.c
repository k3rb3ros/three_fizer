#include "include/cbc.h"
#include "include/hash.h"

void cbc_decrypt(int32_t block_size, uint8_t* password, uint64_t* cipher_text, uint64_t size)
{
    uint8_t* key = hash(password, block_size/8, block_size);

	free(key);
}

void cbc_encrypt(int32_t block_size, uint8_t* password, uint64_t* plain_text, uint64_t size)
{
	uint8_t* key = hash(password, block_size/8, block_size);
    printf("%02x ", key);
	free(key);
}

void cbc_teardown()
{
    
}

void init_state()
{
    
}

