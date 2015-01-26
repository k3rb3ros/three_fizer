#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbcEncrypt.h" //cbcXXXXEncrypt() functions
#include "tfHeader.h" //stripIV()
#include "threefishApi.h" //ThreefishKey_t type

bool encryptHeader(ThreefishKey_t* key, 
                   const uint64_t* header);

#endif 
