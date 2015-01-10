#ifndef DEBUG_H

#define DEBUG_H
#define INDENT "    "

#include <stdio.h> //printf()
#include <stdint.h> //uintxx_t types

    //a function to print every byte in a buffer as a 2 digit hex number in a readable way
    void showBuff(uint64_t size, const void* buffer);

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
        #define pBuff(size, buffer) showBuff(size, buffer)
    #endif
    #ifndef DEBUG3
        #define pd3(...) do {} while(0)
        #define pBuff(...)
    #endif

#endif
