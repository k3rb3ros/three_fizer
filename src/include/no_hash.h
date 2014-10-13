#ifndef NO_HASH_H
#define NO_HASH_H

#include <stdint.h> //uint types
#include <stdio.h> //NULL, printf

uint64_t* set_key(uint8_t* password, uint64_t length, uint64_t state_size);
//Used to set the key directly from the user entered password or buffered password file (Note this will cause a weekening of cryptographic strength if the password is less then the state size of the cipher and truncation will occur if the password is longer then the state size of the cipher)

#endif
