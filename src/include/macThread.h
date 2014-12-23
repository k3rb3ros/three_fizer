#ifndef MACTHREAD_H
#define MACTHREAD_H

#include <pthread.h> //pthread_mutex_t 
#include <stdbool.h> //bool type
#include <stdint.h> //uintxx types
#include "chunkQueue.h" //queue type and support functions
#include "debug.h" //pdebug()
#include "error.h" //error codes
#include "mac.h" //MacCtx_t type, checkMAC() and genMAC()
#include "pthread.h" //pthread_mutex_lock() pthread_mutex_unlock()
#include "threefishApi.h" //ThreefishKey_t type
#include "tfHeader.h" //checkHeader()

typedef struct
{
    bool* mac_status;
    bool* running;
    MacCtx_t* mac_context;
    pthread_mutex_t* in_mutex;
    pthread_mutex_t* out_mutex;
    queue* in;
    queue* out;
    ThreefishKey_t* tf_key;
    uint32_t* error;
    uint64_t* file_size;
} macParams;

void* authenticateMAC(void* parameters);

void* generateMAC(void* parameters);

void setUpMacParams(macParams* params,
              bool* mac_status, 
              bool* running, 
              MacCtx_t* mac_context, 
              pthread_mutex_t* in_mutex,
              pthread_mutex_t* out_mutex,
              queue* in,
              queue* out, 
              ThreefishKey_t* tf_key,
              uint32_t* error,
              uint64_t* file_size);

#endif

