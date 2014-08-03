#ifndef CBC_H
#define CBC_H

#include "threefishApi.h" //threefish encrypt and decrypt functions
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //free()

//Cipher block chaining mode for threefish encryption and decryption
//Skein is used internally to hash the password into a full length key

void cbc_decrypt(int32_t block_size, uint8_t* pw, uint64_t* buffer, uint64_t size);

void cbc_encrypt(int32_t block_size, uint8_t* pw, uint64_t* buffer, uint64_t size);

void cbc_teardown();

void init_state();

#endif
