#ifndef READTHREAD_H
#define READTHREAD_H

#include <pthread.h> //pthread_mutex_t type
#include <stdbool.h> //bool
#include "chunk.h" //chunk type and createChunk()
#include "chunkQueue.h" //queue type and enque()
#include "error.h" //error codes
#include "pad.h" //pad()

typedef struct
{
    const arguments* args;
    bool* running;
    pthread_mutex_t* mutex;
    queue* out;
    uint32_t* error;
} readParams;

void setUpReadParams(readParams* params, 
                     const arguments* args, 
                     bool* running,
                     pthread_mutex_t* mutex,
                     queue* out,
                     uint32_t* error);

void* queueFile(void* parameters);

#endif
