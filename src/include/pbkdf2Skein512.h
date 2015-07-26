#ifndef PBKDF2SKEIN512_H
#define PBKDF2SKEIN512_H

#include <stdint.h>
#include <string.h>
#include "endianConvert.h"
#include "skeinApi.h"

const static SkeinSize_t SKEIN512 = (SkeinSize_t)512;

void PBKDF2_SKEIN512(const uint8_t* passwd, size_t pw_len,
                     const uint8_t* salt, size_t salt_len,
                     uint64_t c, uint8_t* buff, size_t dkLen);

#endif /* end pbkdf2Skein512_h */
