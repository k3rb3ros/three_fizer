#ifndef FILEIO_H
#define FILEIO_H

#include <stdbool.h> //bool type
#include <stdio.h> //fread(), fwrite() and fclose(), calloc()
#include <stdint.h> //uint types
#include <stdlib.h> //calloc()
#include <unistd.h> //fseek() 

int seek_point;

bool exists(const char* fname);

FILE* openForBlockRead(const char* fname);

FILE* openForBlockWrite(const char* fname);

bool writeBlock(uint8_t* data, uint64_t data_size, FILE* write);

bool writeFile(const char* fname, uint8_t* data, uint64_t data_size);

uint8_t* readFile(const char* fname);

uint8_t* readBlock(uint64_t data_size, FILE* read);

#endif
