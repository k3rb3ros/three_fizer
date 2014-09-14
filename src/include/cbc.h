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
* Used internally to cbc decrypt 256bit blocks
*/
void cbc256Decrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* cipher_text, uint64_t num_blocks);

/*
* Used internally to cbc decrypt 512bit blocks
*/
void cbc512Decrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* cipher_text, uint64_t num_blocks);

/*
* Used internally to cbc decrypt 1024 blocks
*/
void cbc1024Decrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* cipher_text, uint64_t num_blocks);

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
