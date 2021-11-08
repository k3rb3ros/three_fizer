cmake_minimum_required (VERSION 2.6)

#C files for Skein/Threefish
set (skein_threefish_c_src
    skeinApi.c
    skeinBlockNo3F.c
    skein.c
    threefish1024Block.c
    threefish256Block.c
    threefish512Block.c
    threefishApi.c
    )

add_library(${SKEIN3FISH_LIB} STATIC ${skein_threefish_c_src})
