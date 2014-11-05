#ifndef CHUNK_H
#define CHUNK_H

#include <stdbool.h> //bool type
#include <stdint.h> //uint and specified int size types
#include <stdio.h> //NULL Macro
#include <stdlib.h> //calloc()

/* defines the chunk structure used to pass groups of encrypted and unencrypted blocks around functions that manipulate them.
* As well as a queue used to store chunks that are being read, encrypted, decrypted or written
*/

#define ENCRYPT 1
#define DECRYPT 2
#define INIT 255
#define WRITE 3

typedef struct
{
     uint8_t action;
     uint64_t* data;
     uint64_t data_size;
} chunk;

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

chunk* createChunk();

chunk* front(queue* q);

queue* createQueue(int max_elements);

void destroyChunk(chunk* chunk);

void destroyQueue(queue* q);

#endif
