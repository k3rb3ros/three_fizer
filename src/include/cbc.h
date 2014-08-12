#ifndef CBC_H
#define CBC_H

#include "hash.h"
#include "random.h"
#include "threefishApi.h" //threefish encrypt and decrypt functions
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //free()

//Cipher block chaining mode for threefish encryption and decryption
//Skein is used internally to hash the password into a full length key

/*
* This function assumes cipher_text has been allocated and is a padded multiple of block size
*/
void cbcDecryptInPlace(SkeinSize_t skein_size, uint8_t* key, uint64_t* cipher_text, uint64_t length);

/*
* This function assumes plain_text has been allocated and is a padded multiple of block size
*/
void cbcEncryptInPlace(SkeinSize_t skein_size, uint8_t* key, uint64_t* plain_text, uint64_t length);

#endif
