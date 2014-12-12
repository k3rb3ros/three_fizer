#ifndef CRYPTOTHREAD_H
#define CRYPTOTHREAD_H

#include "chunkQueue.h" //queue data type
#include "debug.h" //pdebug()
#include "encrypt.h" //encryptHeader(), encryptInPlace()
#include "pad.h" //getNumBlocks()
#include "threefishApi.h" //ThreefishKey_t

typedef struct
{
    const arguments* args;
    bool* running;
    ThreefishKey_t* tf_key;
    queue* in;
    queue* out;
    uint32_t* error;
} cryptParams;

void* decryptQueue(void* parameters);

void* encryptQueue(void* parameters);

void setUpCryptoParams(cryptParams* params,
                      const arguments* args,
                      bool* running,
                      ThreefishKey_t* tf_key,
                      queue* in,
                      queue* out,
                      uint32_t* error);

#endif
