#include "include/tfHeader.h"

bool checkHeader(ThreefishKey_t* key, uint64_t* iv, uint64_t* header, uint64_t* file_size)
{
    const uint32_t block_byte_size = (key->stateSize/8);
    //DecryptInBuffer(key, 1, iv, header, state_size); //TODAY
    
    if(header[block_byte_size+0] == MAGIC_NUMBER && 
       header[block_byte_size+2] == key->stateSize)//if the header check passes
    {
        *file_size = header[block_byte_size+1]; //store the unpadded file size
        return true;
    }
    return false;
}

/**********************************************************************
* The Header is twice the block size of the cipher                    *
* |####IV####|MAGIC_NUMBER|DATA_SIZE|STATE_SIZE|RESERVED|PADDING|     *
***********************************************************************/
uint64_t* genHeader(ThreefishKey_t* key, uint64_t data_size, uint64_t* iv)
{
    const uint32_t block_byte_size = (key->stateSize/8);
    uint64_t* header = NULL;

    if(iv != NULL && key != NULL && data_size > 1)
    {
        header = calloc(sizeof(uint64_t), block_byte_size*2); //allocate memory for the header

        memcpy(header, getRand(key->stateSize), block_byte_size); //copy in the initialization vector
        header[block_byte_size+0] = MAGIC_NUMBER;
        header[block_byte_size+1] = data_size;
        header[block_byte_size+2] = (uint64_t) key->stateSize;
        header[block_byte_size+3] = RESERVED;
    }

    return header;
}
