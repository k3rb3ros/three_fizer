#ifndef CBCDECRYPT_H
#define CBCDECRYPT_H

#include "cbc.h" //Cipher size macros
#include "debug.h" //pd3()
#include <stdint.h> //uintxx_t types

#include "print.h" 

/*
* Used internally to cbc decrypt 256bit blocks
*/
void cbc256Decrypt(ThreefishKey_t* key,
                   const uint64_t* iv,
                   uint64_t* cipher_text,
                   const uint64_t num_blocks);

/*
* Used internally to cbc decrypt 512bit blocks
*/
void cbc512Decrypt(ThreefishKey_t* key,
                   const uint64_t* iv,
                   uint64_t* cipher_text,
                   const uint64_t num_blocks);

/*
* Used internally to cbc decrypt 1024 blocks
*/
void cbc1024Decrypt(ThreefishKey_t* key,
                    const uint64_t* iv,
                    uint64_t* cipher_text,
                    const uint64_t num_blocks);

/*
* This function assumes padding and allocation has already been taken care of
*/
void decryptInPlace(ThreefishKey_t* key,
                    const uint64_t* chain,
                    uint64_t* plain_text,
                    const uint64_t num_blocks);

#endif
