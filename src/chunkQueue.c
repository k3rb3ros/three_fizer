#include "include/chunkQueue.h"

//pops the first queued element out of the queue
bool deque(queue* q)
{
    if(q->size <= 0) { return false; }
    else
    {
        q->size --;
        q->elements[q->head] = NULL; //remove the ptr to the element
        q->head = (q->head + 1) % q->capacity;
    }
    return true;
}

//enqueue the chunk passed in
bool enque(chunk* chunk, queue* q)
{
    if(q->size == q->capacity || chunk == NULL) { return false; }
    else
    {
        q->size ++;
        q->tail = (q->tail + 1) % q->capacity;
        q->elements[q->tail] = chunk;
    }
    return true;
}

//Put a done flag into the Queue telling all threads using it that there operation on queued data is complete
inline bool queueDone(queue* q)
{
    pdebug("queueDone()\n");
    chunk* done = createChunk();
    done->action = DONE;

    if(enque(done, q)) { return true; }

    return false;
}

//generate a header with the arguments given and queue it into the que passed in
bool queueHeader(const arguments* args, queue* out)
{
    pdebug("queueHeader()\n");
    bool success = false;
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8);
    uint64_t* iv = (uint64_t*)getRand((uint64_t) args->state_size);

    chunk* header = createChunk();
    header->action = ENCRYPT;
    header->data = genHeader(iv, args->file_size, args->state_size);
    header->data_size = 2*block_byte_size;

    if(header->data != NULL) //check that allocate succeeded
    {
        while(enque(header, out) != true);
        success = true;
    }

    if(iv != NULL) { free(iv); }
    return success;
}

inline bool queueIsFull(queue* q)
{
   return q->size == q->capacity;
}

//return the first queued element (if any)
chunk* front(queue* q)
{
    chunk* chunk = NULL;
    if(q->size > 0)
    {
       chunk = q->elements[q->head];
    }

    return chunk;
}

//allocate memory for the queue and set all its default values
queue* createQueue(const int max_elements)
{
    queue* q = NULL;
    q = calloc(1, sizeof(queue));
    q->capacity = max_elements;
    q->size = 0;
    q->head = 0;
    q->tail = -1;
    q->elements = calloc(max_elements, sizeof(chunk*));

    for (int i=0; i<max_elements; ++i)
    {
        q->elements[i] = NULL;
    }

    return q;
}

inline void destroyQueue(queue* q)
{
   if(q != NULL)
   {
       while(q->size > 0) //pop any/all chunks in the q and free them
       {
           destroyChunk(front(q));
           deque(q);
       }
       free(q->elements); //free the elements chunk*[]
       free(q); //free the q
   }
}

