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
#define WRITE 3

#define MAX_QUE_SIZE 25

typedef struct
{
     uint8_t action;
     uint64_t* data;
     uint64_t data_size;
} chunk;

typedef struct
{
    int16_t capacity;
    int16_t size;
    int16_t head;
    int16_t tail;
    chunk** elements;
} queue;

bool enque(chunk* chunk, queue* q);

bool deque(queue* q);

chunk* front(queue* q);

queue* create_queue(uint16_t max_elements);

#endif
