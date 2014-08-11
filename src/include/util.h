#ifndef UTIL_H
#define UTIL_H

#include "skeinApi.h"
#include <string.h> //strcmpr()

//Invalid argument type
#define BADARG -1

//Skein/Threefish block sizes
#define SAFE 256
#define SECURE 512
#define FUTURE_PROOF 1024

//Compile time size of our lookup table
#define N_BLOCK_LOOKUP sizeof(block_lookup)/sizeof(keyBlock_t)

typedef struct 
{
    char* key;
    SkeinSize_t skein_size;
} keyBlock_t;

static keyBlock_t block_lookup[] = //alookup table containing skein/threefish cipher sizes
{
    { "256", Skein256 }, { "512", Skein512 }, { "1024", Skein1024 },
    { "SAFE", Skein256 }, { "SECURE", Skein512 }, { "FUTUREPROOF", Skein1024 },
    { "safe", Skein256 }, { "secure", Skein512 }, { "futureproof", Skein1024 },
    { "Safe", Skein256 }, { "Secure", Skein512 }, { "FutureProof", Skein1024 },
    { "FUTURE_PROOF", Skein1024 }, { "future_proof", Skein1024 }, { "Future_Proof", Skein1024 }
};

SkeinSize_t getSkeinSize(char* key);

#endif
