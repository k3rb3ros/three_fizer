#include "include/hash.h"

uint8_t* hash(uint8_t* input, uint32_t digest_length, SkeinSize_t skein_size)
{
    struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = calloc(digest_length, sizeof(uint8_t));

    skeinCtxPrepare(&skein_state, skein_size);
    skeinInit(&skein_state, skein_size);
    skeinUpdate(&skein_state, input, strlen((char*) input));
    skeinFinal(&skein_state, (uint8_t*)digest);

    return digest;
}
