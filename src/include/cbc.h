#ifndef CBC_H
#define CBC_H

#include "skeinApi.h"
#include "threefishApi.h"

#include <stdint.h>
#include <stdio.h>

//TODO Finish these functions
//Cipher block chaining mode for threefish encryption and decryption
//Skein is used internally to hash the password into a full length key

struct SkeinCtx skein_state; //Skein state

ThreefishKey_t key; //Threefish key

uint64_t* hash_password(uint8_t* password);

void cbc_decrypt(uint8_t* pw, uint64_t* buffer, uint64_t size);

void cbc_encrypt(uint8_t* pw, uint64_t* buffer, uint64_t size);

void init_state();

void teardown();

#endif
