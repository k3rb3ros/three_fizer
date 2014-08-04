#include "include/pad.h"

uint64_t getPadSize(uint64_t plain_text_size, uint32_t block_size)
{
    if(plain_text_size % block_size == 0)
    { return plain_text_size; }

    uint64_t padSize = plain_text_size;

    while(padSize % block_size != 0) 
    { padSize++; }
    return padSize;  
}
