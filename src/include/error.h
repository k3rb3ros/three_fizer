#ifndef ERROR_H
#define ERROR_H

#include <stdio.h> //perror()

/*************************
* Compile Time Constants *
**************************/

#define ARG_PARSING_ERROR -1
#define CIPHER_OPERATION_FAIL -3
#define FILE_IO_FAIL -4
#define FILE_TOO_SMALL -5
#define HEADER_CHECK_FAIL -6
#define INVALID_PASSWORD_FILE -7
#define INVALID_TARGET_FILE -8
#define KEY_GENERATION_FAIL -9
#define MAC_CHECK_FAIL -10
#define MAC_GENERATION_FAIL -11
#define MEMORY_ALLOCATION_FAIL -12
#define PASSWORD_ASK_ERROR -13
#define PASSWORD_TOO_SHORT -14
#define QUEUE_OPERATION_FAIL -15
#define TOO_FEW_ARGUMENTS -16

/************
* Functions *
*************/

//Output a human readable message to make sense of the error code
void printError(int error);

#endif
