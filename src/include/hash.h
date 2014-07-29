#ifndef HASH_H
#define HASH_H

#include "skeinApi.h"
#include <stdint.h>
#include <string.h>

SkeinSize_t SkeinSize(int block_size);

uint8_t* hash(uint8_t* input, uint32_t digest_length, int32_t block_size);

#endif
