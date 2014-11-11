#ifndef CHUNK_H
#define CHUNK_H

#include <stdbool.h> //bool type
#include <stdint.h> //uint and specified int size types
#include <stdio.h> //NULL Macro
#include <stdlib.h> //calloc()

/***********************************
* Compile Time Numerical Constants *
************************************/

#define INIT 255

/* defines the chunk structure used to pass groups of encrypted and unencrypted blocks around functions that manipulate them.
*/

/******************
* Data Structures *
*******************/

typedef struct
{
     uint8_t action; //The action to be performed on the queded chunk
     uint64_t* data; //A pointer to the data queued
     uint64_t data_size; //the size of the data in bytes
} chunk;

chunk* createChunk();

void destroyChunk(chunk* chunk);

#endif
