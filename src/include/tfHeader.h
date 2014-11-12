#ifndef TFHEADER_H
#define TFHEADER_H

#include <stdbool.h> //bool type
#include <stdint.h> //int types
#include "skeinApi.h" //SkeinSize_t type
#include "threefishApi.h" //ThreefishKey_t type

#define MAGIC_NUMBER 0x0909202009092009ULL
#define RESERVED 0x7256736572766564ULL

/************
* Functions *
*************/

bool checkHeader(uint64_t* header, uint64_t* file_size, 
                 uint32_t state_size); //returns true if the header is valid (correct valid) and sets file_size

uint64_t* genHeader(uint64_t* iv, uint64_t data_size, 
                    uint32_t state_size); //generates a header indicating the file was encrypted with this program and indicateing how big the plain_text is

#endif
