#ifndef READTHREAD_H
#define READTHREAD_H

#include <stdbool.h> //bool
#include "chunk.h" //chunk type and createChunk()
#include "chunkQueue.h" //queue type and enque()
#include "error.h" //error codes
#include "pad.h" //pad()

typedef struct
{
    const arguments* args;
    bool* running;
    queue* out;
    uint32_t* error;
} readParams;

void setUpReadParams(readParams* params, 
                     const arguments* args, 
                     bool* running, 
                     queue* out,
                     uint32_t* error);

void* queueFile(void* parameters);

#endif
