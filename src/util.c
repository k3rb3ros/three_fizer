#include "util.h"

SkeinSize_t getSkeinSize(char* key)
{
    for(unsigned long i=0; i<N_BLOCK_LOOKUP; ++i)
    {
        keyBlock_t* sym = &block_lookup[i];
        if(strcmp(sym->key, key) ==0) { return sym->skein_size; }
    }
    return NULL; //Should not happen
}
