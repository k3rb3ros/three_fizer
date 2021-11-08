#ifndef CHUNKTESTS_H
#define CHUNKTESTS_H

#include "chunkQueue.h" //chunk and queue structures and supporting functions 
#include "debug.h" //pdebug()
#include <assert.h> //assert()
#include <stdlib.h> //malloc()
#include <stdio.h> //printf()
#include <string.h> //memcpy()

#define DATA1 "Data 1"
#define DATA2 "Data 2"
#define DATA3 "Data 3"
#define DATA4 "Data 4"
#define DATA5 "Data 5"

#define TEST_QUE_SIZE 5

void runChunkTests();

#endif
