#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include <stdio.h>

#define INDENT "    "

//print the contents of the buffer as an columns of 4 hexidecmal digits
void Show08(uint64_t cnt, const uint8_t* b);

#endif
