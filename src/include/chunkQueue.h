#ifndef CHUNKQUEUE_H
#define CHUNKQUEUE_H

#include <pthread.h> //pthread_mutex_t type
#include <stdbool.h> //bool types
#include <stdint.h> //std uint types
#include "arguments.h" //arguments structure
#include "chunk.h" //chunk type
#include "debug.h" //pdebug()
#include "queueActions.h" //que actions
#include "random.h" //getRand()

/*defines the queue data structure used to queue asymmetric IO and Crypto operation*/

/*********************************
* Compile Time Numeric Constants *
**********************************/

//Queue constants
//number of chunks in QUEUE
#define QUE_SIZE 10 //10*50MB*3Queues = 150MB max queue usage
//size of each queue chunk
#define MAX_CHUNK_SIZE 51200000 //50MB

/******************
* Data Structures *
*******************/

typedef struct
{
    int capacity;
    int size;
    int head;
    int tail;
    chunk** elements;
} queue;

bool deque(queue* q);

bool enque(chunk* chunk, queue* q);

bool queueDone(queue* q);

bool queueIsFull(queue* q);

chunk* front(queue* q);

queue* createQueue(const int max_elements);

void destroyQueue(queue* q);

#endif
