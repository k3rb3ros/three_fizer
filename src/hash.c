#include "include/hash.h"

uint8_t* hash(uint8_t* input, uint32_t digest_length, SkeinSize_t skein_size)
{
    struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = NULL;

    if(input != NULL)
    {
        digest = calloc(digest_length, sizeof(uint8_t));    
        skeinCtxPrepare(&skein_state, skein_size); //Tell skein what size its state is
        skeinInit(&skein_state, digest_length*8); //Init our hash
        skeinUpdate(&skein_state, input, strlen((char*) input)); //Generate our hash
        skeinFinal(&skein_state, (uint8_t*)digest); //Put the results in our buffer and return it
    }

    return digest;
}
