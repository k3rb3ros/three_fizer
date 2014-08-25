#ifndef PAD_H
#define PAD_H

#include "debug.h" //pdebug()
#include "skeinApi.h" //SkeinSize_t
#include <stdint.h> //stdint types
#include <stdlib.h> //realloc()

uint64_t getNumBlocks(uint64_t plain_text_size, SkeinSize_t skein_size);

uint64_t getPadSize(uint64_t plain_text_size, SkeinSize_t skein_size);

uint64_t* pad(uint8_t* input, uint64_t input_length, SkeinSize_t skein_size);

#endif
