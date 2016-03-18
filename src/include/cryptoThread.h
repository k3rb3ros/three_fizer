#ifndef CRYPTOTHREAD_H
#define CRYPTOTHREAD_H

#include <pthread.h> //pthread_mutex_t type
#include <time.h> //nanosleep()
#include "cbc.h" //getChainInBuffer()
#include "chunkQueue.h" //queue data type
#include "debug.h" //pdebug()
#include "decrypt.h" //decryptHeader(), decryptInPlace()
#include "encrypt.h" //encryptHeader(), encryptInPlace()
#include "error.h" //error codes
#include "pad.h" //getNumBlocks()
#include "progressBar.h" //progress_t type
#include "threefishApi.h" //ThreefishKey_t
#include "wait.h" //wait_interval

typedef struct
{
    const arguments* args;
    bool* running;
    int32_t* error;
    pthread_mutex_t* in_mutex;
    pthread_mutex_t* out_mutex;
    queue* in;
    queue* out;
    progress_t* progress;
    ThreefishKey_t* tf_key;
} cryptParams;

void* decryptQueue(void* parameters);

void* encryptQueue(void* parameters);

void setUpCryptoParams(cryptParams* params,
                       const arguments* args,
                       bool* running,
                       ThreefishKey_t* tf_key,
                       pthread_mutex_t* in_mutex,
                       pthread_mutex_t* out_mutex,
                       queue* in,
                       queue* out,
                       progress_t* progress,
                       int32_t* error);

#endif
