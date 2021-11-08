#pragma once

#include <stdbool.h> //bool type
#include <stdint.h> //uint and specified int size types
#include <stdio.h> //NULL Macro
#include <stdlib.h> //calloc()
#include "debug.h" //pdebug()

#define INIT 255

/* defines the chunk structure used to pass groups of encrypted and unencrypted blocks around functions that manipulate them.
*/
typedef struct
{
     uint8_t action; //The action to be performed to the data
     uint64_t* data; //A pointer to the data to be operated on
     size_t data_size; //the size of the data in bytes
} chunk;

chunk* createChunk();

void destroyChunk(chunk* chunk);
