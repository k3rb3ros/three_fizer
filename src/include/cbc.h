#pragma once

#include "debug.h" //pdebug()
#include "random.h" //getRand()
#include "skein3fish/skeinApi.h" //SkeinSize_t
#include "skein3fish/threefishApi.h" //threefish encrypt and decrypt functions
#include <stdint.h> //uint types
#include <stdio.h> //printf(), memcpy()
#include <string.h> //memmove()

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
                      uint64_t* buffer,
                      const uint64_t num_blocks,
                      const SkeinSize_t state_size);

/*
* used to get the last block from the previous cbc operation in cases where 
* continuous chaining is interupted for encryption(so the chain can exist in place)
*/
uint64_t* getChainInPlace(const uint64_t* cipher_text,
                          const uint64_t num_blocks, 
                          const SkeinSize_t state_size);
