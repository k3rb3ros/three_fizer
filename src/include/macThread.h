#ifndef MACTHREAD_H
#define MACTHREAD_H

#include <pthread.h> //pthread_mutex_t 
#include <stdbool.h> //bool type
#include <stdint.h> //uintxx types
#include "chunkQueue.h" //queue type and support functions
#include "debug.h" //pdebug()
#include "error.h" //error codes
#include "mac.h" //MacCtx_t type, checkMAC() and genMAC()

typedef struct
{
    bool* mac_status;
    bool* running;
    MacCtx_t* mac_context;
    pthread_mutex_t* in_mutex;
    pthread_mutex_t* out_mutex;
    queue* in;
    queue* out;
    uint32_t* error;
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
              uint32_t* error);

#endif
