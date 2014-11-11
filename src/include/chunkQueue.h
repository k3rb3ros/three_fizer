#ifndef CHUNKQUEUE_H
#define CHUNKQUEUE_H

#include "chunk.h"

/*defines the queue data structure used to queue asymmetric IO and Crypto operation*/

/*********************************
* Compile Time Numeric Constants *
**********************************/

#define ENCRYPT 1
#define DECRYPT 5
#define DONE 4
#define GEN_MAC 2
#define WRITE 3

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

queue* createQueue(int max_elements);

void destroyQueue(queue* q);

#endif
