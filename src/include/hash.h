#ifndef HASH_H
#define HASH_H

#include <stdint.h> //uint types
#include <stdlib.h> //calloc()
#include <string.h> //strlen()
#include "fileIO.h" //openForRead(), readBytes(), getFileSize()
#include "skeinApi.h" //skein functions
#include "util.h" //validSize()

/*************************
* compile time constants *
**************************/
#define HASH_BUFFER_SIZE 5120000 //50MB

/*
 * Returns a ptr to a byte array of @digest_length bytes hashed from @input which
 * must be a null terminated string, the State size of skein used is @stat_size
 */
uint8_t* skeinHash(const uint8_t* input,
                   const uint64_t digest_length,
                   const SkeinSize_t state_size);

/*
 * Hashes the file @fname to a digest of the state size of the cipher to be used as a
 * key for a threefish operation
 */
uint64_t* hashKeyFromFile(const uint8_t* fname, const SkeinSize_t state_size);

/*
 * Returns a ptr to a byte arrary of @state_size bits in length hashed from
 * @input @input_length must be specified as this function will work on non null
 * terminated strings
 */
uint8_t* keyHash(const uint8_t* input,
                 const uint64_t input_length,
                 const SkeinSize_t state_size);

#endif
