#ifndef CIPHER_H
#define CIPHER_H

#include "cbc.h"
#include "fileIO.h" //openForRead openForWrite openForBlockRead openForBlockWrite
#include "skeinApi.h" //SkeinSize_t
#include "stdint.h" //uint8_t, int32_t, uint64_t
#include <stdio.h> //FILE*

typedef struct //arguments passed to argp containing argz (all non optional args)
{
    bool encrypt;
    bool free;
    char* argz;
    size_t argz_len;
    SkeinSize_t skein_size;
    uint8_t* password;
    uint64_t pw_length;
} arguments;

typedef struct
{
    FILE* file;
    uint8_t* key;
    uint64_t* padded_input;
} cipher;

int run_cipher(arguments* args, char* filename);

bool decrypt(uint8_t* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t skein_size);

bool encrypt(uint8_t* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t skein_size);


#endif
