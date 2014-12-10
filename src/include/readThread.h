#ifndef READTHREAD_H
#define READTHREAD_H

#include <stdbool.h> //bool
#include "chunk.h" //chunk type and createChunk()
#include "chunkQueue.h" //queue type and enque()
#include "pad.h" //pad()

typedef struct
{
    const arguments* args;
    bool* error;
    bool* running;
    queue* out;
} readParams;

void setUpReadParams(readParams*, 
                     const arguments* args, 
                     bool* error, 
                     bool* running, 
                     queue* out);

void* queueFile(void* parameters);

#endif
