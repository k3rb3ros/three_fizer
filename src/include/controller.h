#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <arguments.h> //arguments structure
#include <stdint.h> //uint types
#include "chunk.h" //chunk types and queue types
#include "debug.h" //pdeubug()
#include "fileIO.h" //block read and write operations
#include "hash.h" //hashing sf_hash()
#include "noHash.h" //set_key()
#include "threefishApi.h" //ThreefishKey_t

#define SUCCESS 1
#define FILE_IO_FAIL 2
#define HEADER_CHECK_FAIL 3
#define HMAC_CHECK_FAIL 4

//number of chunks in QUEUE
#define QUE_SIZE 10
//size of each queue chunk
#define MAX_CHUNK_SIZE 524288  

/******************
* Data structures *
*******************/

//define an arbitrary tweak so other versions of threefish w/ the same key won't produce the same cipher text
static uint64_t tf_tweak[2] = { 0x5368616C6C206E6ULL, 0X26520646976756CULL };

/************
* Functions *
*************/

int32_t runThreefizer(arguments* args);

#endif
