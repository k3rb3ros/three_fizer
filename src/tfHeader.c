#include "include/tfHeader.h"

/*Checks if the header is valid which is a quick and easy way to tell if the encryption key and block size are correct, with out having to try and decrypt the entire file
*/
bool checkHeader(const uint64_t* header,
                 uint64_t* file_size,
                 const uint32_t state_size)
{
    pdebug("checkHeader()\n");
    const uint32_t iv_offset = (state_size/64);
    
    if(header != NULL && file_size != NULL && validSize(state_size))
    {
        pdebug("Header{ ");
        pdebug("%lu, ", header[iv_offset+0]);
        pdebug("%lu, ", header[iv_offset+1]);
        pdebug("%lu, ", header[iv_offset+2]);
        pdebug("%lu ", header[iv_offset+3]);
        pdebug("}\n");

        if(header[iv_offset+0] == MAGIC_NUMBER && 
           header[iv_offset+2] == state_size)//if the header check passes
        {
            *file_size = header[iv_offset+1]; //store the unpadded file size
            pdebug("Header check passed, header is valid\n");
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
uint64_t* genHeader(const uint64_t* iv,
                    const uint64_t data_size,
                    const uint32_t state_size)
{
    pdebug("genHeader()\n");
    const uint32_t block_byte_size = (state_size/8);
    const uint32_t block_uint64_size = (state_size/64);
    uint64_t* header = NULL;

    if(iv != NULL && validSize(state_size) && data_size > 1)
    {
        header = calloc(2*block_uint64_size, sizeof(uint64_t)); //allocate memory for the header

        memcpy(header, iv, block_byte_size);
        header[block_uint64_size+0] = MAGIC_NUMBER; 
        header[block_uint64_size+1] = data_size; 
        header[block_uint64_size+2] = state_size;
        header[block_uint64_size+3] = RESERVED;
        pdebug("Header{ ");
        pdebug("%lu, ", header[block_uint64_size+0]);
        pdebug("%lu, ", header[block_uint64_size+1]);
        pdebug("%lu, ", header[block_uint64_size+2]);
        pdebug("%lu ", header[block_uint64_size+3]);
        pdebug("}\n");
    }

    return header;
}

/*Knowing the internal structure of the header it is possible to return a pointer directly to the data of the header ignoring the iv*/
inline uint64_t* stripIV(const uint64_t* header, const uint64_t state_size)
{
    return header + (state_size/64);
}
