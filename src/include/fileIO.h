#ifndef FILEIO_H
#define FILEIO_H

#include "debug.h" //pdebug()
#include <stdbool.h> //bool type
#include <stdio.h> //fread(), fwrite() and fclose(), calloc()
#include <stdint.h> //uint types
#include <stdlib.h> //calloc()
#include <sys/stat.h> //stat()
#include <unistd.h> //fseek() 

bool exists(const char* fname);

FILE* openForBlockRead(const char* fname);

FILE* openForBlockWrite(const char* fname);

bool writeBlock(const uint8_t* data, const uint64_t data_size, const FILE* write);

bool writeFile(const char* fname, uint8_t* data, uint64_t data_size);

uint8_t* readBlock(const uint64_t data_size, const FILE* read);

uint8_t* readFile(const char* fname);

uint64_t getSize(const char* fname);

void terminateFile(const FILE* write);
#endif
