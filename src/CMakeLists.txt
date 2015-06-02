cmake_minimum_required (VERSION 2.6)

find_package(Threads REQUIRED)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

#optional sets to force Clang as compiler
set (CMAKE_C_COMPILER clang)
set (CMAKE_CXX_COMPILER clang++)

#C files for threefizer
set (threefizer_c_src
     arguments.c
     cbc.c
     cbcDecrypt.c
     cbcEncrypt.c
     chunk.c
     chunkQueue.c
     controller.c
     cryptoThread.c
     debug.c
     decrypt.c
     encrypt.c
     error.c
     fileIO.c
     hash.c
     key.c
     mac.c
     macThread.c
     noHash.c
     pad.c
     progressBar.c
     random.c
     readThread.c
     skeinApi.c
     skeinBlockNo3F.c
     skein.c
     tfHeader.c
     threefish1024Block.c
     threefish512Block.c
     threefish256Block.c
     threefishApi.c
     threefizer.c
     util.c
     writeThread.c
    )

#C files for threefizer tests
set (tests_c_src
    )

#Optional debugging flags
#add_definitions("-D DEBUG DEBUG2 DEBUG3")

add_executable (${project} ${threefizer_c_src})
target_link_libraries(${project} "pthread")
