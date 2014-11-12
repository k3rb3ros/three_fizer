#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include <stdio.h>

#define INDENT "    "

//print the contents of the buffer as columns of 4 pairs of hexidecmal digits
void ShowBuff(uint64_t cnt, const uint8_t* b);

#endif
