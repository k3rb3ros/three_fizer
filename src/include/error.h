#ifndef ERROR_H
#define ERROR_H

#include <stdio.h> //perror()

/*************************
* Compile Time Constants *
**************************/

#define CIPHER_OPERATION_FAIL -1
#define FILE_IO_FAIL -2
#define FILE_TOO_SMALL -3
#define HEADER_CHECK_FAIL -4
#define KEY_GENERATION_FAIL -5
#define MAC_CHECK_FAIL -6
#define MAC_GENERATION_FAIL -7
#define MEMORY_ALLOCATION_FAIL -8
#define QUEUE_OPERATION_FAIL -9

/************
* Functions *
*************/

//Output a human readable message to make sense of the error code
void printError(int error);

#endif
