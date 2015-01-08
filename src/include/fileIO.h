#ifndef FILEIO_H
#define FILEIO_H

#include "debug.h" //pdebug()
#include <fcntl.h> //open()
#include <stdbool.h> //bool type
#include <stdio.h> //calloc()
#include <stdint.h> //uintxx_t and intxx_t types
#include <stdlib.h> //calloc()
#include <sys/stat.h> //stat()
#include <unistd.h> //fseek() write() 

bool exists(const char* fname);

bool writeBlock(const uint8_t* data, const uint64_t data_size, FILE* write);

bool writeBytes(const uint8_t* data, const uint64_t data_size, int64_t write_fd);

int openForRead(const char* fname);

int openForWrite(const char* fname);

uint8_t* readBytes(const uint64_t data_size, int64_t read_rd);

uint8_t* readEntireFile(const char* fname);

uint8_t* readFile(const char* fname);

uint64_t getFileSize(const char* fname);

#endif
