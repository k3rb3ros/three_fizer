#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbc.h" //cbcXXXXEncrypt() functions
#include "threefishApi.h" //ThreefishKey_t type
#include "tfHeader.h" //stripIV()

/*
bool decryptChunk(const ThreefishKey_t* key,
                  const uint64_t* chain,
                  uint64_t* plain_text,
                  const uint64_t num_blocks);
*/

bool decryptHeader(ThreefishKey_t* key,
                   uint64_t* header);

void decryptInPlace(ThreefishKey_t* key,
                    const uint64_t* chain,
                    uint64_t* plain_text,
                    const uint64_t num_blocks);

#endif
