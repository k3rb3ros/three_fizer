#ifndef CHUNKTESTS_H
#define CHUNKTESTS_H

#include "threefizer/chunk.h"
#include "threefizer/debug.h"
#include <assert.h>
#include <stdlib.h> //malloc
#include <string.h> //memcpy

#define DATA1 "Data 1"
#define DATA2 "Data 2"
#define DATA3 "Data 3"
#define DATA4 "Data 4"
#define DATA5 "Data 5"

#define TEST_QUE_SIZE 5

void runChunkTests();

void testCreateChunkQueue();

void testChunkEnque();

void testChunkDeque();

void testChunkLimit();

#endif
