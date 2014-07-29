#include "include/hash.h"

SkeinSize_t SkeinSize(int32_t block_size)
{
    switch(block_size)
    {
        case 256:
        return Skein256;
        break;

	case 512:
	return Skein512;
	break;

	case 1024:
	return Skein1024;
	break;
	
	default :
	return Skein512;
	break;
    }
}

uint8_t* hash(uint8_t* input, uint32_t digest_length, int32_t block_size)
{
	struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = calloc(digest_length, sizeof(uint8_t));

    skeinCtxPrepare(&skein_state, SkeinSize(block_size));
    skeinInit(&skein_state, block_size/8);
    skeinUpdate(&skein_state, input, strlen((char*) input));
    skeinFinal(&skein_state, (uint8_t*)digest);

    return digest;
}
