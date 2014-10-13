#include "include/hash.h"

uint8_t* hash(uint8_t* input, uint64_t digest_length, SkeinSize_t state_size)
{
    struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = NULL;

    if(input != NULL)
    {
        digest = calloc(digest_length+1, sizeof(uint8_t)); //+1 to ensure digest is a null terminated string    
        skeinCtxPrepare(&skein_state, state_size); //Tell skein what size its state is
        skeinInit(&skein_state, digest_length*8); //Init our hash
        skeinUpdate(&skein_state, input, strlen((char*)input)); //Generate our hash
        skeinFinal(&skein_state, (uint8_t*)digest); //Put the results in our buffer and return it
    }

    return digest;
}

uint8_t* sf_hash(uint8_t* input, uint64_t input_length, SkeinSize_t state_size)
{
    struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = NULL;

    if(input != NULL)
    {
        digest = calloc((state_size/8), sizeof(uint8_t)); //allocate the digest buffer    
        skeinCtxPrepare(&skein_state, state_size); //Tell skein what size its state is
        skeinInit(&skein_state, state_size); //Init our hash
        skeinUpdate(&skein_state, input, input_length); //Generate our hash
        skeinFinal(&skein_state, (uint8_t*)digest); //Put the results in our buffer and return it
    }

    return digest;
}
