#pragma once

#include <pthread.h> //pthread_mutex_t type
#include <stdbool.h> //bool types
#include <stdint.h> //std uint types
#include "arguments.h" //arguments structure
#include "chunk.h" //chunk type
#include "commonConstants.h" //QUE_SIZE, MAX_CHUNK_SIZE
#include "debug.h" //pdebug()
#include "queueActions.h" //que actions
#include "random.h" //getRand()

/* defines the queue data structure used to queue asymmetric IO and Crypto operations*/

typedef struct
{
    size_t capacity;
    size_t size;
    int_fast32_t head;
    int_fast32_t tail;
    chunk** elements;
} queue;

bool deque(queue* q);

bool enque(chunk* chunk, queue* q);

bool queueDone(queue* q);

bool queueIsFull(queue* q);

chunk* front(queue* q);

queue* createQueue(const size_t max_elements);

void destroyQueue(queue* q);
