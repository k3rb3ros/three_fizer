#include <stdbool.h>        //bool type
#include <stdint.h>         //uint8_t type

//Compile time size of our lookup table
#define N_BLOCK_LOOKUP sizeof(block_lookup)/sizeof(key_t)
#define N_HEX_LOOKUP 15

typedef struct
{
    uint8_t binary;
    uint8_t hex;
} hex_t;

typedef struct
{
    char* key;
    SkeinSize_t skein_size;
} key_t;

//a lookup table containing skein/threefish cipher sizes.
//Note these are just for parsing block size input not cryptographic operations
static key_t block_lookup[] =
        {
                { "256", Skein256 }, { "512", Skein512 }, { "1024", Skein1024 },
                { "SAFE", Skein256 }, { "SECURE", Skein512 }, { "FUTUREPROOF", Skein1024 },
                { "safe", Skein256 }, { "secure", Skein512 }, { "futureproof", Skein1024 },
                { "Safe", Skein256 }, { "Secure", Skein512 }, { "FutureProof", Skein1024 },
                { "FUTURE_PROOF", Skein1024 }, { "future_proof", Skein1024 }, { "Future_Proof", Skein1024 }
        };

//a lookup table for converting binary nibbles(4 bits) to hex
static hex_t hex_lookup[] =
        {
                { 0, '0' }, { 1, '1' }, { 2, '2' }, { 3, '3' },
                { 4, '4' }, { 5, '5' }, { 6, '6' }, { 7, '7' },
                { 8, '8' }, { 9, '9' }, { 10, 'A' }, { 11, 'B' },
                { 12, 'C' }, { 13, 'D' }, { 14, 'E' }, { 15, 'F' },
        };