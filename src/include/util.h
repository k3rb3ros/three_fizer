#ifndef UTIL_H
#define UTIL_H

#include "arguments.h"      //arguments struct
#include "debug.h"          //pdebug()
#include "skeinApi.h"       //SkenSize_t
#include "threefishApi.h"   //ThreefishKey_t
#include <stdbool.h>        //bool type
#include <stdint.h>         //uint8_t type
#include <stdlib.h>         //free()
#include <stdio.h>          //printf()
#include <string.h>         //strcmp()
#include <termios.h>        //tcsetattr(), tcgetattr()

//Invalid argument type
#define BADARG -1

//Skein/Threefish block sizes
#define SAFE 256
#define SECURE 512
#define FUTURE_PROOF 1024

#define BUFF_SIZE 1024

bool isGreaterThanThreeBlocks(const arguments* args);

bool validSize(const size_t size);

SkeinSize_t getSkeinSize(const char* key);

uint8_t* binToHex(uint8_t* src, uint64_t size);

void askPassword(arguments* args);

#endif