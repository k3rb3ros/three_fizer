#pragma once

#include <stdint.h> //uintxx_t types
#include <stdio.h> //printf()

#define INDENT "    "

//print the contents of the buffer as columns of 4 pairs of hexidecmal digits
void showBuff(uint64_t cnt, const uint8_t* b);
