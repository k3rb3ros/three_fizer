#ifndef CHUNKQUEUE_H
#define CHUNKQUEUE_H

#include "chunk.h"

/*defines the queue data structure used to queue asymmetric IO and Crypto operation*/

/*********************************
* Compile Time Numeric Constants *
**********************************/

//Actions
#define ENCRYPT 1
#define CHECK_HEADER 2
#define DECRYPT 3
#define GEN_MAC 4
#define DONE 5
#define MAC 6
#define WRITE 7

//Queue constants
//number of chunks in QUEUE
#define QUE_SIZE 10
//size of each queue chunk
#define MAX_CHUNK_SIZE 524288

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

bool queueIsFull(queue* q);

chunk* front(queue* q);

queue* createQueue(const int max_elements);

void destroyQueue(queue* q);

#endif
