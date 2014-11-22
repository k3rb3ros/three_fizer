#include "include/chunk.h"

//allocate storage and init default values for a chunk
chunk* createChunk()
{
    chunk* ch = calloc(1, sizeof(chunk));
    ch->action = INIT;
    ch->data = NULL;
    ch->data_size = INIT;
    return ch;
}

inline void destroyChunk(chunk* chunk) 
{
    if(chunk != NULL)
    {
        pdebug("Freeing chunk of size %lu\n", chunk->data_size);
        free(chunk->data);
        free(chunk);
    }
}
