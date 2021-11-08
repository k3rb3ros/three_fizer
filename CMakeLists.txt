cmake_minimum_required (VERSION 2.6)
project (threefizer C)

#Compiler flags
set (CMAKE_C_COMPILER clang)

#Optimization and debug flags
set (DEFINES
     "POSIX_C_SOURCE")
#add_definitions (-DDEBUG1 -DDEBUG2 -DDEBUG3)
#add_definitions (-DDEBUG1)
#add_definitions (-DDEBUG3)

include (CheckIncludeFiles)

set (APP_INC_PATH ${PROJECT_SOURCE_DIR}/src)
set (PCG_INC_PATH ${PROJECT_SOURCE_DIR}/src/pcg)
set (SCRYPT_INC_PATH ${PROJECT_SOURCE_DIR}/src/scrypt)
set (SKEIN_INC_PATH ${PROJECT_SOURCE_DIR}/src/skein3fish)

find_library (THREAD_LIB pthread)

check_include_files (stdio.h HAVE_STDIO_H)
check_include_files (stdint.h HAVE_STDINT_H)
check_include_files (stdlib.h HAVE_STDLIB_H)
check_include_files (string.h HAVE_STRING_H)
check_include_files (pthread.h HAVE_PTHREAD_H)

add_subdirectory (src)
