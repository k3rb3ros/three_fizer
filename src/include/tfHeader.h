#ifndef TFHEADER_H
#define TFHEADER_H

#include <stdbool.h>        //bool type
#include <stdint.h>         //uint types
#include <stdlib.h>         //calloc()
#include <string.h>         //memcpy()
#include "chunk.h"          //chunk type
#include "chunkQueue.h"     //queue type
#include "decrypt.h"        //decryptHeader()
#include "debug.h"          //pdebug()
#include "queueActions.h"   //queue actions
#include "random.h"         //getRand()
#include "skeinApi.h"       //SkeinSize_t type
#include "threefishApi.h"   //ThreefishKey_t type
#include "util.h"           //validSize()

#define MAGIC_NUMBER 0x0909202009092009ULL
#define RESERVED 0x7256736572766564ULL

/************
* Functions *
*************/

//returns true if the header is valid (correct valid) and sets file_size
// to the size of the unencrypted file read from the header
bool checkHeader(const uint64_t* header, 
                 uint64_t* file_size, 
                 const SkeinSize_t state_size);

bool headerIsValid(ThreefishKey_t* tf_key,
                   chunk* header,
                   uint64_t* file_size);

bool queueHeader(const arguments* args, queue* out);

//return a pointer to the data of the header w/out an IV
uint64_t* stripIV(const uint64_t* header, const uint64_t state_size);

#endif