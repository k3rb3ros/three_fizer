#include "include/cbc.h"

bool getChainInBuffer(const uint64_t* cipher_text,
                      uint64_t* buffer, 
                      const uint64_t num_blocks, 
                      const SkeinSize_t state_size)
{
    const uint64_t block_byte_size = ((uint64_t)state_size/8);
    const uint64_t offset = ((uint64_t)state_size/64) * (num_blocks-1);
    const uint64_t* chain = (cipher_text + offset);

    if(memmove(buffer, chain, block_byte_size) == NULL) { return false; }

    return true;
}

inline uint64_t* getChainInPlace(const uint64_t* cipher_text,
                                 const uint64_t num_blocks,
                                 const SkeinSize_t state_size)
{
    const uint64_t offset = ((uint64_t)state_size/64) * (num_blocks-1);

    return ((uint64_t*)cipher_text + offset);
}
