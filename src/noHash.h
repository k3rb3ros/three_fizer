#pragma once

#include <stdint.h> //uint types
#include <stdio.h> //NULL, printf()
#include <stdlib.h> //calloc()
#include <string.h> //memcpy()
#include "fileIO.h" //openForRead(), readBytes(), getFileSize, exists()
#include "skein3fish/skeinApi.h" //SkeinSize_t

uint64_t* noHashKey(const uint8_t* password, const uint64_t length, const SkeinSize_t state_size);
/* Used to set the key directly from the user entered password (Note this will cause a weekening
 * of cryptographic strength of the password if it is less then the state size of the cipher and
 * truncation will occur if the password is longer then the state size of the cipher)
 * This serves no useful purpose other then exposing the functionality to bypass the key
 * the key generation code and test the cipher directly.
 */

uint64_t* noHashBlockFromFile(const uint8_t* fname, const SkeinSize_t state_size);
/* Used to set a block directly from a file input by the user. The file must be at least as big
 * is the block or the function will return * NULL. If the file is larger then the byte size of
 * the block then the block returned will be truncated to the size of the block
 */
