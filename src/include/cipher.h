#ifndef CIPHER_H
#define CIPHER_H

#include "arguments.h" //arguments structure
#include "cbc.h" //cbc_encrypt and cbc_decrypt
#include "fileIO.h" //openForRead openForWrite openForBlockRead openForBlockWrite
#include "no_hash.h" //set_key()
#include "pad.h" //pad()
#include "skeinApi.h" //SkeinSize_t
#include "stdint.h" //uint8_t, int32_t, uint64_t
#include <stdio.h> //FILE*

#define MAGIC_NUMBER 0x0909202009092009ULL
#define RESERVED 0x7256736572766564ULL

typedef struct
{
    FILE* file;
    uint8_t* key;
    uint64_t* padded_input;
} cipher;

bool decrypt(const char* filename, uint64_t* key, uint64_t pw_length, SkeinSize_t state_size); //a function that encapsulate all decryption operations

bool check_header(ThreefishKey_t* key, uint64_t* iv, uint64_t* header, uint64_t* file_size, SkeinSize_t state_size); //returns true if the header is valid (correct valid) and sets file_size

bool encrypt(const char* filename, uint64_t* key, uint64_t pw_length, SkeinSize_t state_size); //a function that encapsulates all encryption operations

int run_cipher(arguments* args, char* filename); //runs the appropriate function based on the arguments from argp

uint64_t* gen_header(ThreefishKey_t* key, uint64_t data_size, uint64_t* iv, SkeinSize_t state_size); //generates an encrypted header containing a magic number, the filesize and state side

/*
* This function assumes cipher_text has been allocated and is a padded multiple of block size
*/
void DecryptInPlace(ThreefishKey_t* key, uint64_t num_blocks, uint64_t* iv, uint64_t* cipher_text, SkeinSize_t state_size);

/*
* This function assumes plain_text has been allocated and is a padded multiple of block size
*/
void EncryptInPlace(ThreefishKey_t* key, uint64_t num_blocks, uint64_t* iv, uint64_t* plain_text, SkeinSize_t state_size);

#endif
