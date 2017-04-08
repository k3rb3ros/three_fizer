#pragma once

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbcEncrypt.h" //cbcXXXXEncrypt() functions
#include "tfHeader.h" //stripIV()
#include "skein3fish/threefishApi.h" //ThreefishKey_t type

bool encryptHeader(ThreefishKey_t* key, const uint64_t* header);
