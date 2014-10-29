#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <arguments.h> //arguments structure
#include <stdint.h> //uint types
#include "chunk.h" //chunk types and queue types
#include "debug.h" //pdeubug()
#include "hash.h" //hashing sf_hash()
#include "noHash.h" //set_key()
#include "threefishApi.h" //ThreefishKey_t

#define QUE_SIZE 25

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
