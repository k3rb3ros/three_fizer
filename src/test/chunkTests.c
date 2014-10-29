#include <stdio.h>
#include "include/chunkTests.h"

static void testCreateChunkQueue()
{
    pdebug("testCreateChunkQueue()\n");
    queue* q = create_queue(TEST_QUE_SIZE);
    assert(q->capacity == TEST_QUE_SIZE);
    assert(q->size == 0);
    assert(q->head == 0);
    assert(q->tail == -1);

    free(q);
}

static void testChunkEnque()
{
    pdebug("testChunkEnque()\n");

    static chunk chunk1;
    chunk1.action = WRITE;
    chunk1.data = malloc(7);
    memcpy(chunk1.data, DATA1, 7);
    chunk1.data_size = strlen(chunk1.data);   

    queue* q = create_queue(TEST_QUE_SIZE);
    enque(&chunk1, q);    

    assert(strcmp(q->elements[0]->data, DATA1) == 0);
    free(chunk1.data);
    free(q);
}

static void testChunkDeque()
{
    pdebug("testChunkDeque()\n");

    static chunk chunk1;
    chunk1.action = WRITE;
    chunk1.data = malloc(7);
    memcpy(chunk1.data, DATA1, 7);
    chunk1.data_size = strlen(chunk1.data);   

    queue* q = create_queue(TEST_QUE_SIZE);
    assert(enque(&chunk1, q) == true);
    assert(deque(q) == true);
    assert(q->size == 0);
    assert(q->elements[0] == NULL);

    free(chunk1.data);
    free(q); 
}

static void testChunkFront()
{
    pdebug("testFront()\n");

    static chunk chunk1;
    chunk1.action = WRITE;
    chunk1.data = malloc(7);
    memcpy(chunk1.data, DATA1, 7);
    chunk1.data_size = strlen(chunk1.data);   

    queue* q = create_queue(TEST_QUE_SIZE);
    enque(&chunk1, q);
    chunk* test1 = front(q);
    assert(test1->action == WRITE); 
    assert(strcmp(test1->data, DATA1) == 0);
    assert(test1->data_size == strlen(DATA1));

    free(chunk1.data);
    free(q);
}

static void testChunkLimit()
{
    pdebug("testChunkLimit()\n");

    static chunk chunk1;
    chunk1.action = WRITE;
    chunk1.data = malloc(7);
    memcpy(chunk1.data, DATA1, 7);
    chunk1.data_size = 6;   

    static chunk chunk2;
    chunk2.action = WRITE;
    chunk2.data = malloc(7);
    memcpy(chunk2.data, DATA2, 7);
    chunk2.data_size = 6;

    static chunk chunk3;
    chunk3.action = WRITE;
    chunk3.data = malloc(7);
    memcpy(chunk3.data, DATA3, 7);
    chunk3.data_size = 6;

    static chunk chunk4;
    chunk4.action = WRITE;
    chunk4.data = malloc(7);
    memcpy(chunk4.data, DATA4, 7);
    chunk4.data_size = 6;

    static chunk chunk5;
    chunk5.action = WRITE;
    chunk5.data = malloc(7);
    memcpy(chunk5.data, DATA5, 7);
    chunk5.data_size = 6;

    static chunk one_too_many;
    one_too_many.action = ENCRYPT;
    one_too_many.data = NULL;
    one_too_many.data_size = -1;

    queue* q = create_queue(TEST_QUE_SIZE);
    
    enque(&chunk1, q);
    enque(&chunk2, q);
    enque(&chunk3, q);
    enque(&chunk4, q);
    enque(&chunk5, q);
    assert(enque(&one_too_many, q) == false);

    free(chunk1.data);
    free(chunk2.data);
    free(chunk3.data);
    free(chunk4.data);
    free(chunk5.data);
    free(q);
}

static void testChunkQueueBuffer()
{
    pdebug("testChunkQueueBuffer()\n");
   
    static chunk chunk1;
    chunk1.action = WRITE;
    chunk1.data = malloc(7);
    memcpy(chunk1.data, DATA1, 7);
    chunk1.data_size = 6;

    static chunk chunk2;
    chunk2.action = WRITE;
    chunk2.data = malloc(7);
    memcpy(chunk2.data, DATA2, 7);
    chunk2.data_size = 6;

    static chunk chunk3;
    chunk3.action = WRITE;
    chunk3.data = malloc(7);
    memcpy(chunk3.data, DATA3, 7);
    chunk3.data_size = 6;

    static chunk chunk4;
    chunk4.action = WRITE;
    chunk4.data = malloc(7);
    memcpy(chunk4.data, DATA4, 7);
    chunk4.data_size = 6;

    static chunk chunk5;
    chunk5.action = WRITE;
    chunk5.data = malloc(7);
    memcpy(chunk5.data, DATA5, 7);
    chunk5.data_size = 6;

    queue* q = create_queue(TEST_QUE_SIZE);

    enque(&chunk5, q);
    enque(&chunk4, q);
    enque(&chunk3, q);
    enque(&chunk2, q);
    enque(&chunk1, q);

    deque(q);
    enque(&chunk3, q);
    assert(enque(&chunk3, q) == false);

    deque(q);
    deque(q);

    enque(&chunk1, q);
    enque(&chunk2, q);
    assert(enque(&chunk1, q) == false);

    deque(q);
    deque(q);
    deque(q);
 
    enque(&chunk3, q);
    enque(&chunk4, q);
    enque(&chunk5, q);

    pdebug("Contents of queue { ");
    pdebug("%s, ", front(q)->data);
    deque(q);
    pdebug("%s, ", front(q)->data);
    deque(q);
    pdebug("%s, ", front(q)->data);
    deque(q);
    pdebug("%s, ", front(q)->data);
    deque(q);
    pdebug("%s ", front(q)->data);
    deque(q);

    pdebug(" }\n");
 
    free(chunk1.data);
    free(chunk2.data);
    free(chunk3.data);
    free(chunk4.data);
    free(chunk5.data);
    free(q);    
}

void runChunkTests()
{
    testCreateChunkQueue();
    testChunkEnque();
    testChunkDeque();
    testChunkFront();
    testChunkLimit();
    testChunkQueueBuffer();
}