#ifndef CIPHER_H
#define CIPHER_H

#include "arguments.h" //arguments structure
#include "cbc.h" //cbc_encrypt and cbc_decrypt
#include "fileIO.h" //openForRead openForWrite openForBlockRead openForBlockWrite
#include "pad.h" //pad()
#include "skeinApi.h" //SkeinSize_t
#include "stdint.h" //uint8_t, int32_t, uint64_t
#include <stdio.h> //FILE*

typedef struct
{
    FILE* file;
    uint8_t* key;
    uint64_t* padded_input;
} cipher;

int run_cipher(arguments* args, char* filename);

bool decrypt(const char* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t skein_size);

bool encrypt(const char* filename, uint8_t* password, uint64_t pw_length, SkeinSize_t skein_size);


#endif
