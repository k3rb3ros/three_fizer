#ifndef DEBUG_H

#define DEBUG_H
#define INDENT "    "

#include "print.h" //showBuff()
#include <stdio.h> //printf()
#include <stdint.h> //uintxx_t types

    #ifdef DEBUG
        #define pdebug(format, ...) printf(format, ##__VA_ARGS__)
    #endif
    #ifndef DEBUG
        #define pdebug(...) do {} while(0)
    #endif

    #ifdef DEBUG2
        #define pd2(format, ...) printf(format, ##__VA_ARGS__)
    #endif
    #ifndef DEBUG2
        #define pd2(...) do {} while(0)
    #endif

    #ifdef DEBUG3
        #define pd3(format, ...) printf(format, ##__VA_ARGS__)
        //#define pBuff(size, buffer) showBuff(size, buffer)
    #endif
    #ifndef DEBUG3
        #define pd3(...) do {} while(0)
        #define pBuff(...) do {} while(0)
    #endif

#endif
