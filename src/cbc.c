#include "include/cbc.h"

SkeinSize_t SkeinSize()
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
	default:
	    return Skein512;
	break;	
    }
}

void cbc_decrypt(uint8_t* password, uint64_t* cipher_text, uint64_t size)
{
    init_state();
    hash_password(password);
}

void cbc_encrypt(uint8_t* password, uint64_t* plain_text, uint64_t size)
{
    init_state();
    hash_password(password);
}

void cbc_teardown()
{
    if(key_buffer != NULL) free(key_buffer);
}

void hash_password(uint8_t* password)
{
    skeinCtxPrepare(&skein_state, SkeinSize());
    skeinInit(&skein_state, block_size/8);
    skeinUpdate(&skein_state, password, strlen((char*)password));
    skeinFinal(&skein_state, (uint8_t*)key_buffer);
}

void init_state()
{
    key_buffer = calloc(block_size/64, sizeof(uint64_t));
    switch(block_size)
    {
        case 256:
	break;
	case 512:
	break;
	case 1024:
	break;
    }
}

