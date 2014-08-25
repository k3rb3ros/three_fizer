#ifndef CBC_H
#define CBC_H

#include "debug.h"
#include "hash.h"
#include "random.h"
#include "threefishApi.h" //threefish encrypt and decrypt functions
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //free(), exit()

#define SAFE_SLICE 4
#define SECURE_SLICE 8
#define FUTURE_PROOF_SLICE 16

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

/*
* Used internally to cbc encrypt 256bit blocks
*/
void cbc256Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks);

/*
* Used internally to cbc encrypt 512bit blocks
*/
void cbc512Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks);

/*
* Used internally to cbc encrypt 1024bit blocks
*/
void cbc1024Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks);

#endif
