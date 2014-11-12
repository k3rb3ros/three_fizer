#ifndef RANDOM_H
#define RANDOM_H

#include "fileIO.h" //exists(), openForBlockRead()
#include <stdint.h> //uint types
#include <stdio.h> //frpintf

#define HWRNG "/dev/hw_random"
#define PSRNG "/dev/urandom"

uint8_t* getRand(uint64_t const size);

#endif
