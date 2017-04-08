#pragma once

#include <stdio.h> //printf()
#include <stdint.h> //uintxx_t types
#include "commonConstants.h" //INDENT
#include "print.h" //showBuff()

/* this header contains compile time macros to overload printf to print debugging
 * information
 */

#ifdef DEBUG1
    #define pdebug(format, ...) printf(format, ##__VA_ARGS__)
#endif
#ifndef DEBUG1
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
    #define pBuff(...) do {} while(0)
#endif
