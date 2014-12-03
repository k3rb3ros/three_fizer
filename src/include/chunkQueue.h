#ifndef CHUNKQUEUE_H
#define CHUNKQUEUE_H

#include <stdbool.h> //bool types
#include <stdint.h> //std uint types
#include "arguments.h" //arguments structure
#include "chunk.h" //chunk type
#include "queueActions.h" //que actions
#include "random.h" //getRand()
#include "tfHeader.h" //genHeader()

/*defines the queue data structure used to queue asymmetric IO and Crypto operation*/

/*********************************
* Compile Time Numeric Constants *
**********************************/

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

bool queueDone(queue* q);

bool queueHeader(const arguments* args, queue* out);

bool queueIsFull(queue* q);

chunk* front(queue* q);

queue* createQueue(const int max_elements);

void destroyQueue(queue* q);

#endif
