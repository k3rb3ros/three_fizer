#ifndef ERROR_H
#define ERROR_H

#include <stdio.h> //perror()

/*************************
* Compile Time Constants *
**************************/

#define ARG_PARSING_ERROR -1
#define CIPHER_OPERATION_FAIL -2
#define FILE_IO_FAIL -3
#define FILE_TOO_SMALL -4
#define HEADER_CHECK_FAIL -5
#define INVALID_PASSWORD_FILE -6
#define INVALID_TARGET_FILE -7
#define KEY_GENERATION_FAIL -8
#define MAC_CHECK_FAIL -9
#define MAC_GENERATION_FAIL -10
#define MEMORY_ALLOCATION_FAIL -11
#define PASSWORD_TOO_SHORT -12
#define QUEUE_OPERATION_FAIL -13
#define TOO_FEW_ARGUMENTS -14

/************
* Functions *
*************/

//Output a human readable message to make sense of the error code
void printError(int error);

#endif
