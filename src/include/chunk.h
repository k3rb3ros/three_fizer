#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

//defines the chunk structure used to pass groups of encrypted and unecnrypted blocks around functions that manipulate them

typedef struct
{
     uint8_t action;
     uint64_t* data;
     uint64_t data_size;
} chunk;

#endif
