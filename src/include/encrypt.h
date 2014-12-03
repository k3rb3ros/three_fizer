#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbc.h" //cbcXXXXEncrypt() functions
#include "tfHeader.h" //stripIV()
#include "threefishApi.h" //ThreefishKey_t type

bool encryptHeader(ThreefishKey_t* key, 
                   const uint64_t* header);

void encryptInPlace(ThreefishKey_t* key,
                    const uint64_t* chain,
                    uint64_t* plain_text,
                    const uint64_t num_blocks);
#endif 
