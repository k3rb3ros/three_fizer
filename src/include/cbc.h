#ifndef CBC_H
#define CBC_H

#include "debug.h"
#include "hash.h"
#include "random.h"
#include "threefishApi.h" //threefish encrypt and decrypt functions
#include <stdint.h> //uint types
#include <stdio.h> //printf(), memcpy

#define SAFE_SLICE 4
#define SECURE_SLICE 8
#define FUTURE_PROOF_SLICE 16

//Cipher block chaining mode for threefish encryption and decryption
//Skein is used internally to hash the password into a full length key

/*
* used to get the last block from the cipher text from the previous cbc 
* operation in cases where the cipher text is being decrypted and needs to be 
* stored in its own buffer
*/
bool getChainInBuffer(const uint64_t* cipher_text,
                      const uint64_t* buffer,
                      const uint64_t num_blocks,
                      const uint32_t state_size);

/*
* used to get the last block from the previous cbc operation in cases where 
* continuous chaining is interupted for encryption(so the chain can exist in place)
*/
uint64_t* getChainInPlace(const uint64_t* cipher_text,
                          const uint64_t num_blocks, 
                          const uint32_t state_size);

/*
* Used internally to cbc decrypt 256bit blocks
*/
void cbc256Decrypt(const ThreefishKey_t* key, 
                   const uint64_t* iv, 
                   uint64_t* cipher_text, 
                   const uint64_t num_blocks);

/*
* Used internally to cbc decrypt 512bit blocks
*/
void cbc512Decrypt(const ThreefishKey_t* key, 
                   const uint64_t* iv, 
                   uint64_t* cipher_text, 
                   const uint64_t num_blocks);

/*
* Used internally to cbc decrypt 1024 blocks
*/
void cbc1024Decrypt(const ThreefishKey_t* key, 
                    const uint64_t* iv, 
                    uint64_t* cipher_text, 
                    const uint64_t num_blocks);

/*
* Used internally to cbc encrypt 256bit blocks
*/
void cbc256Encrypt(const ThreefishKey_t* key, 
                   const uint64_t* iv, 
                   uint64_t* plain_text, 
                   const uint64_t num_blocks);

/*
* Used internally to cbc encrypt 512bit blocks
*/
void cbc512Encrypt(const ThreefishKey_t* key, 
                   const uint64_t* iv, 
                   uint64_t* plain_text, 
                   const uint64_t num_blocks);

/*
* Used internally to cbc encrypt 1024bit blocks
*/
void cbc1024Encrypt(const ThreefishKey_t* key, 
                    const uint64_t* iv, 
                    uint64_t* plain_text, 
                    const uint64_t num_blocks);

#endif
