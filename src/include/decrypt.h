#pragma once

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbcDecrypt.h" //cbcXXXXDecrypt() functions
#include "skein3fish/threefishApi.h" //ThreefishKey_t type
#include "tfHeader.h" //stripIV()

bool decryptHeader(ThreefishKey_t* key, uint64_t* header);
