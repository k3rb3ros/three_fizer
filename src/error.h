#pragma once

#include <stdio.h> //perror()

#define ARG_PARSING_FAIL -1
#define CIPHER_OPERATION_FAIL -2
#define FILE_IO_FAIL -3
#define FILE_TOO_SMALL -4
#define HEADER_CHECK_FAIL -5
#define INVALID_PASSWORD_FILE -6
#define INVALID_TARGET_FILE -7
#define INTEGER_OVERFLOW -8
#define KEY_GENERATION_FAIL -9
#define MAC_CHECK_FAIL -10
#define MAC_GENERATION_FAIL -11
#define MEMORY_ALLOCATION_FAIL -12
#define PASSWORD_TOO_SHORT -14
#define QUEUE_OPERATION_FAIL -15
#define SCRYPT_INVALID_VALUE -16
#define TOO_FEW_ARGUMENTS -17

void printError(int error);
