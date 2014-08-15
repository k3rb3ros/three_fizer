#include "include/hash.h"

uint8_t* hash(uint8_t* input, uint32_t digest_length, SkeinSize_t skein_size)
{
    printf("In hash(%s, %d, %d)", input, digest_length, skein_size);
    struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = NULL;

    if(input != NULL)
    {
        digest = calloc(digest_length+1, sizeof(uint8_t)); //+1 to ensure digest is a null terminated string    
        skeinCtxPrepare(&skein_state, skein_size); //Tell skein what size its state is
        skeinInit(&skein_state, digest_length*8); //Init our hash
        skeinUpdate(&skein_state, input, digest_length); //Generate our hash
        skeinFinal(&skein_state, (uint8_t*)digest); //Put the results in our buffer and return it
    }

    return digest;
}
