#include "include/noHash.h"

uint64_t* noHashKey(const uint8_t* password, const uint64_t length, const SkeinSize_t state_size)
{
    uint64_t* key = NULL;    
    const uint64_t block_byte_size = state_size/8;
    key = calloc(block_byte_size, sizeof(uint8_t));

    if(length < state_size)
    {
        printf("***Warning*** You have turned off password hashing and entered a password that is smaller then the state size of the cipher. A loss of cryptographic strength of your data will occur\n");
    }

    if(length >= state_size)
    {
        printf("***Warning*** You have turned off password hashing and entered a password that is larger then the state size. Only the first %lu characters of your password will be used as the key. The rest will be truncated.\n", block_byte_size);
    }

    memcpy(key, password, block_byte_size);

    return key;
}

uint64_t* noHashBlockFromFile(const uint8_t* fname, const SkeinSize_t state_size)
{
    uint64_t* block = NULL;
    const uint64_t block_byte_size = state_size/8;

    if(exists(fname) && getFileSize(fname) >= block_byte_size)
    {
	    int fd = openForRead(fname);
	    if(fd > 0)
	    {
	        block = (uint64_t*)readBytes(block_byte_size, fd);
	        close(fd); 
	    }
    }

    return block;
}
