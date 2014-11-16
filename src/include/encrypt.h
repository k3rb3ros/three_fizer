#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbc.h" //cbcXXXXEncrypt() functions
#include "threefishApi.h" //ThreefishKey_t type

bool encryptHeader(const ThreefishKey_t* key, 
                   const uint64_t* header);

void encryptInPlace(const ThreefishKey_t* key,
                    const uint64_t* chain,
                    const uint64_t* plain_text,
                    const uint64_t num_blocks);
#endif 
