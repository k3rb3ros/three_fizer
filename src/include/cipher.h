#ifndef CIPHER_H
#define CIPHER_H

#include "arguments.h" //arguments structure
#include "cbc.h" //cbc_encrypt and cbc_decrypt
#include "fileIO.h" //openForRead openForWrite openForBlockRead openForBlockWrite
#include "noHash.h" //set_key()
#include "pad.h" //pad()
#include "skeinApi.h" //SkeinSize_t
#include <stdint.h> //uint8_t, int32_t, uint64_t
#include <stdio.h> //FILE*

#define MAGIC_NUMBER 0x0909202009092009ULL
#define RESERVED 0x7256736572766564ULL

/************
* Functions *
*************/

int runCipher(arguments* args, char* filename); //runs the appropriate function based on the arguments from argp

/*
* This function assumes cipher_text has been allocated and is a padded multiple of block size
*/
void DecryptInPlace(ThreefishKey_t* key, uint64_t num_blocks, uint64_t* iv, uint64_t* cipher_text, SkeinSize_t state_size);

/*
* This function assumes plain_text has been allocated and is a padded multiple of block size
*/
void EncryptInPlace(ThreefishKey_t* key, uint64_t num_blocks, uint64_t* iv, uint64_t* plain_text, SkeinSize_t state_size);

#endif
