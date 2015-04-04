#ifndef PAD_H
#define PAD_H

#include "debug.h"      //pdebug()
#include "skeinApi.h"   //SkeinSize_t
#include <stdio.h>      //perror()
#include <stdint.h>     //stdint types
#include <stdlib.h>     //realloc()
#include <string.h>     //memcpy()

uint64_t getNumBlocks(const uint64_t plain_text_size, const SkeinSize_t skein_size);

uint64_t getPadSize(const uint64_t plain_text_size, const SkeinSize_t skein_size);

uint64_t* pad(const uint8_t* input, const uint64_t input_length, const SkeinSize_t skein_size);

#endif