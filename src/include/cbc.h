#ifndef CBC_H
#define CBC_H

#include "skeinApi.h"
#include "threefishApi.h"
#include "threefizer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TODO Finish these functions
//Cipher block chaining mode for threefish encryption and decryption
//Skein is used internally to hash the password into a full length key

struct SkeinCtx skein_state; //Skein state

uint64_t* key_buffer = NULL;

ThreefishKey_t key; //Threefish key

SkeinSize_t SkeinSize();

void cbc_decrypt(uint8_t* pw, uint64_t* buffer, uint64_t size);

void cbc_encrypt(uint8_t* pw, uint64_t* buffer, uint64_t size);

void cbc_teardown();

void init_state();

void hash_password(uint8_t* password);

#endif
