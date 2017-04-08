#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h> //uint types
#include <stdio.h> //frpintf
#include <time.h> //time()
#include <unistd.h> //close()
#include "fileIO.h" //exists(), openForRead(), readBytes
#include "../pcg/pcg_basic.h" //pcg32_srandom_r() pcg32x2_random_r()

uint8_t* getRand(uint64_t const size);

#endif
