#ifndef FILEIOTESTS_H
#define FILEIOTESTS_H

#include <assert.h> //assert()
#include <stdbool.h> //bool type
#include <stdint.h> //uint64_t type
#include <stdio.h> //printf()
#include <string.h> //strcmp()
#include "threefizer/fileIO.h" //all fileIO operations

#define REF_LINE "abcdefghijklmnopqrstuvwxyz\n"
#define TEST_FILE_LINE_LENGTH 27

void runFileIOTests();

#endif
