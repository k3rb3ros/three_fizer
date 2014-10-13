#ifndef HASH_H
#define HASH_H

#include "skeinApi.h" //skein functions
#include <stdint.h> //uint types
#include <stdlib.h> //calloc()
#include <string.h> //strlen()

/*
* Returns a ptr to a byte array of @digest_length bytes hashed from @input which must be a * null terminated string, the State size of skein used is @stat_size
*/
uint8_t* hash(uint8_t* input, uint64_t digest_length, SkeinSize_t state_size);

/*
* Returns a ptr to a byte arrary of @state_size bits in length hashed from @input @input_length must be specified as this function will work on non null terminated strings
*/
uint8_t* sf_hash(uint8_t* input, uint64_t input_length, SkeinSize_t state_size);
#endif
