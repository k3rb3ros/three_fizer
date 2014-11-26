#ifndef TFHEADER_H
#define TFHEADER_H

#include <stdbool.h> //bool type
#include <stdint.h> //int types
#include "debug.h" //pdebug()
#include "skeinApi.h" //SkeinSize_t type
#include "threefishApi.h" //ThreefishKey_t type

#define MAGIC_NUMBER 0x0909202009092009ULL
#define RESERVED 0x7256736572766564ULL

/************
* Functions *
*************/

bool checkHeader(const uint64_t* header, 
                 uint64_t* file_size, 
                 const SkeinSize_t state_size); //returns true if the header is valid (correct valid) and sets file_size to the size of the unencrypted file read from the header

uint64_t* genHeader(const uint64_t* iv, 
                    const uint64_t data_size, 
                    const uint32_t state_size); //generates a header indicating the file was encrypted with this program and indicateing how big the plain_text is

//return a pointer to the data of the header w/out an IV
uint64_t* stripIV(const uint64_t* header, const uint64_t state_size);

#endif
