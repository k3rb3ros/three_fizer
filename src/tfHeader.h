#pragma once

#include <stdbool.h> //bool type
#include <stdint.h> //uint types
#include <stdlib.h> //calloc()
#include <string.h> //memcpy()
#include "arguments.h" //arguments type
#include "chunk.h" //chunk type
#include "chunkQueue.h" //queue type
#include "decrypt.h" //decryptHeader()
#include "debug.h" //pdebug()
#include "queueActions.h" //queue actions
#include "random.h" //getRand()
#include "skein3fish/skeinApi.h" //SkeinSize_t type
#include "skein3fish/threefishApi.h" //ThreefishKey_t type
#include "util.h" //validSize()

#define MAGIC_NUMBER 0x0909202009092009ULL
#define V1 0x7256736572766564ULL
#define V2 0x5077073467643229ULL

/************
* Functions *
*************/

//return true if the header is valid and sets the int passed into as file_size w/ the original file size
bool checkHeader(const uint64_t* header, 
                 uint64_t* file_size, 
                 const SkeinSize_t state_size); //(correct valid) and sets file_size to the size of the unencrypted file read from the header

bool headerIsValid(ThreefishKey_t* tf_key,
                   chunk* header,
                   uint64_t* file_size);

bool queueHeader(arguments* args, queue* out);

//generate a new IV and put it in arguments
bool genIV(arguments* args);

//read the first block from the file passed in assuming its the IV
//(if not the header check will fail)
bool getIV(arguments* args);

//return a pointer to the data of the header w/out an IV
uint64_t* stripIV(const uint64_t* header, const uint64_t state_size);
