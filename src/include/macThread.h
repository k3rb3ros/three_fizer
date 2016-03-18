#ifndef MACTHREAD_H
#define MACTHREAD_H

#include <pthread.h> //pthread_mutex_t 
#include <stdbool.h> //bool type
#include <stdint.h> //uintxx types
#include <time.h> //nanosleep()
#include "chunkQueue.h" //queue type and support functions
#include "debug.h" //pdebug()
#include "error.h" //error codes
#include "mac.h" //MacCtx_t type, checkMAC() and genMAC()
#include "progressBar.h" //progress_t type
#include "threefishApi.h" //ThreefishKey_t type
#include "tfHeader.h" //checkHeader()
#include "wait.h" //wait_interval

typedef struct
{
    bool* mac_status;
    bool* running;
    bool* valid;
    int32_t* error;
    MacCtx_t* mac_context;
    pthread_mutex_t* in_mutex;
    pthread_mutex_t* out_mutex;
    queue* in;
    queue* out;
    progress_t* progress;
    ThreefishKey_t* tf_key;
    uint64_t* file_size;
} macParams;

void* authenticateMAC(void* parameters);

void* generateMAC(void* parameters);

void setUpMacParams(macParams* params,
                    bool* mac_status, 
                    bool* running,
                    bool* valid, 
                    MacCtx_t* mac_context, 
                    pthread_mutex_t* in_mutex,
                    pthread_mutex_t* out_mutex,
                    queue* in,
                    queue* out, 
		            progress_t* progress,
                    ThreefishKey_t* tf_key,
                    int32_t* error,
                    uint64_t* file_size);

#endif

