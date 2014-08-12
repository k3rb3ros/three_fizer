#ifndef HASH_H
#define HASH_H

#include "skeinApi.h" //skein functions
#include <stdint.h> //uint types
#include <stdlib.h> //calloc()
#include <string.h> //strlen()

/*
* Return an array of bytes of size digest_length using skein with the state size block_size.
* If an invalid block size is entered a block size of 512 bits will be used.
*/
uint8_t* hash(uint8_t* input, uint32_t digest_length, SkeinSize_t skein_size);

#endif