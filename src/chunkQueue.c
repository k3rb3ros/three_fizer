#include "chunkQueue.h"

//pops the first queued element out of the queue
bool deque(queue* q)
{
    if (q->size <= 0) { return false; }
    else
    {
        q->size--;
        q->elements[q->head] = NULL; //remove the ptr to the element
        q->head = (++q->head) % q->capacity;
    }

    return true;
}

//enqueue the chunk passed in
bool enque(chunk* chunk, queue* q)
{
    if (q->size == q->capacity || chunk == NULL) { return false; }
    else
    {
        q->size++;
        q->tail = (++q->tail) % q->capacity;
        q->elements[q->tail] = chunk;
    }

    return true;
}

//Put a done flag into the Queue telling all threads using it that there operation on queued data is complete
inline bool queueDone(queue* q)
{
    chunk* done = createChunk();
    done->action = DONE;

    if (enque(done, q)) { return true; }
  
    pdebug("!!!!!queueDone Failed!!!!!");
    return false;
}

inline bool queueIsFull(queue* q)
{
    return q->size == q->capacity;
}

//return the first queued element (if any)
inline chunk* front(queue* q)
{
    return q->size > 0 ? q->elements[q->head] : NULL;
}

//allocate memory for the queue and set all its default values
queue* createQueue(const size_t max_elements)
{
    queue* q = NULL;
    q = calloc(1, sizeof(queue));
    q->capacity = max_elements;
    q->size = 0;
    q->head = 1;
    q->tail = 0;
    q->elements = calloc(max_elements, sizeof(chunk*));

    for (size_t s=0; s<max_elements; ++s)
    {
        q->elements[s] = NULL;
    }

    return q;
}

inline void destroyQueue(queue* q)
{
   if (q != NULL)
   {
       while (q->size > 0) //pop any chunks in the queue and free them
       {
           destroyChunk(front(q));
           deque(q);
       }

       free(q->elements); //free the elements chunk*[]
       free(q); //free the q
   }
}
