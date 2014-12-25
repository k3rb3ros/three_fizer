#ifndef NOHASH_H
#define NOHASH_H

#include <stdint.h> //uint types
#include <stdio.h> //NULL, printf()
#include <stdlib.h> //calloc()
#include <string.h> //memcpy()

uint64_t* noHashKey(const uint8_t* password, uint64_t length, const uint64_t state_size);
//Used to set the key directly from the user entered password or buffered password file (Note this will cause a weekening of cryptographic strength if the password is less then the state size of the cipher and truncation will occur if the password is longer then the state size of the cipher)

#endif
