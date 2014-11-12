#include "include/tfHeader.h"

bool checkHeader(uint64_t* header, uint64_t* file_size, 
                 uint32_t state_size)
{
    const uint32_t block_byte_size = (state_size/8);
    
    if(header != NULL && file_size != NULL && validSize(state_size))
    {
        if(header[block_byte_size+0] == MAGIC_NUMBER && 
           header[block_byte_size+2] == state_size)//if the header check passes
        {
            *file_size = header[block_byte_size+1]; //store the unpadded file size
            return true;
        }
    }
    return false;
}

/**********************************************************************
* The Header is twice the block size of the cipher                    *
* The first block being the random initialization vector              *
* The second block containg a magic number, the unencrypted file size *
* the cipher state size a reserved word that is essentially a 2nd     *
* magic number and NULL padding if the state size > 256 bits          *
*                  HEADER STRUCTURE                                   *
* |#######################IV#########################|                *
* |MAGIC_NUMBER|DATA_SIZE|STATE_SIZE|RESERVED|PADDING|                *
***********************************************************************/
uint64_t* genHeader(uint64_t* iv, uint64_t data_size, uint32_t state_size)
{
    const uint32_t block_byte_size = state_size/8;
    uint64_t* header = NULL;

    if(iv != NULL && validSize(state_size) && data_size > 1)
    {
        header = calloc(sizeof(uint64_t), block_byte_size*2); //allocate memory for the header

        memcpy(header, iv, block_byte_size);
        header[block_byte_size+0] = MAGIC_NUMBER; 
        header[block_byte_size+1] = data_size; 
        header[block_byte_size+2] = state_size;
        header[block_byte_size+3] = RESERVED;
    }

    return header;
}
