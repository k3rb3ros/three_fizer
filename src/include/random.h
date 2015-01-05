#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h> //uint types
#include <stdio.h> //frpintf
#include <unistd.h> //close()
#include "fileIO.h" //exists(), openForRead(), readBytes

#define HWRNG "/dev/hw_random"
#define PSRNG "/dev/urandom"

uint8_t* getRand(uint64_t const size);

#endif
