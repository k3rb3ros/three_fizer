#pragma once

#include <pthread.h> //pthread_mutex_t type
#include <stdbool.h> //bool
#include <time.h> //nanosleep()
#include <unistd.h> //close()
#include "chunk.h" //chunk type and createChunk()
#include "chunkQueue.h" //queue type and enque()
#include "error.h" //error codes
#include "pad.h" //pad()
#include "progressBar.h" //progress_t type
#include "util.h" //isGreaterThanThreeBlocks()
#include "wait.h" //struct wait_interval

typedef struct
{
    const arguments* args;
    bool* running;
    int32_t* error;
    pthread_mutex_t* mutex;
    progress_t* progress;
    queue* out;
} readParams;

void* queueFileForDecrypt(void* parameters);

void* queueFileForEncrypt(void* parameters);

void* queueFile(void* parameters);

void setUpReadParams(readParams* params, 
                     const arguments* args, 
                     bool* running,
                     pthread_mutex_t* mutex,
                     queue* out,
		             progress_t* progress,
                     int32_t* error);
