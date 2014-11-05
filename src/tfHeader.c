#include "include/tfHeader.h"

bool checkHeader(ThreefishKey_t* key, uint64_t* iv, uint64_t* header, uint64_t* file_size)
{
    //DecryptInBuffer(key, 1, iv, header, state_size); //TODAY

    if(header[0] == MAGIC_NUMBER && header[2] == key->stateSize)//if the header check passes
    {
        *file_size = header[1]; //store the unpadded file size
        return true;
    }
    return false;
}

uint64_t* genHeader(ThreefishKey_t* key, uint64_t data_size, uint64_t* iv)
{
    uint64_t* header = NULL;

    if(iv != NULL && key != NULL && data_size > 1)
    {
        header = calloc(sizeof(uint64_t), (key->stateSize)/64);
        header[0] = MAGIC_NUMBER;
        header[1] = data_size;
        header[2] = (uint64_t) key->stateSize;
        header[3] = RESERVED;
    }

    return header;
}
