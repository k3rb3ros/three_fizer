cmake_minimum_required (VERSION 2.6)

#C files for PCG random number generator
set (pcg_c_src
     pcg_basic.c
    )

add_library(${RNG_LIB} STATIC ${pcg_c_src})
