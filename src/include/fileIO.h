#pragma once

#include "debug.h" //pdebug()
#include <fcntl.h> //open()
#include <stdbool.h> //bool type
#include <stdio.h> //calloc()
#include <stdint.h> //uintxx_t and intxx_t types
#include <stdlib.h> //calloc()
#include <sys/stat.h> //stat()
#include <unistd.h> //fseek() write() 

//return true if the file name passed in is a regular file
bool isFile(const uint8_t* name);

//return true if the the file passed in exists
bool exists(const uint8_t* fname);

//write the given buffer of bytes to the fd passed in
bool writeBytes(const uint8_t* data, const uint64_t data_size, int64_t write_fd);

int openForRead(const uint8_t* fname);

int openForWrite(const uint8_t* fname);

uint8_t* readBytes(const uint64_t data_size, int64_t read_rd);

uint64_t getFileSize(const uint8_t* fname);
