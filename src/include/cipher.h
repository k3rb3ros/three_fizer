#ifndef CIPHER_H
#define CIPHER_H

#include "fileIO.h" //openForRead openForWrite openForBlockRead openForBlockWrite
#include "skeinApi.h" //SkeinSize_t
#include "stdint.h" //uint8_t, int32_t, uint64_t
#include <stdio.h> //FILE*

typedef struct
{
    FILE* file;
    uint8_t* key;
    uint64_t* padded_input;
} cipher;

cipher initCipher(uint8_t* filename, uint8_t* password, SkeinSize_t skein_size);

void decrypt();

void encrypt();

#endif
