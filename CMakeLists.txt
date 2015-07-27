cmake_minimum_required (VERSION 2.6)
project (threefizer C)

#Optimization and debug flags
#add_definitions (-DDEBUG1 -DDEBUG2 -DDEBUG3)
#add_definitions (-DDEBUG1)

include (CheckIncludeFiles)

set (CMAKE_C_FLAGS "-std=c11 -ggdb -Wall -Wextra -pedantic")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_library (THREAD_LIB pthread)

check_include_files (stdio.h HAVE_STDIO_H)
check_include_files (stdint.h HAVE_STDINT_H)
check_include_files (stdlib.h HAVE_STDLIB_H)
check_include_files (string.h HAVE_STRING_H)
check_include_files (pthread.h HAVE_PTHREAD_H)

add_subdirectory (src)
