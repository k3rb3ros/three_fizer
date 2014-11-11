#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdbool.h> //bool types
#include <stdint.h> //uint types
#include "cbc.h" //cbcXXXXEncrypt() functions
#include "threefishApi.h" //ThreefishKey_t type

/*encrypt the plain_text passed into the function in place 
* if this is the first block of encryption chain is the initialization vector
* otherwise chain is the last block of the previous encryption operation
*/
bool encryptChunk(ThreefishKey_t* key, uint64_t* chain, uint64_t* plain_text, uint64_t num_blocks);

#endif 
