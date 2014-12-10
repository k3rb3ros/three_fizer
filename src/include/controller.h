#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <arguments.h> //arguments structure
#include <limits.h> //ULLONG_MAX
#include <pthread.h> //pthread_t
#include <stdint.h> //uint types
#include <stdio.h> //rename()
#include "cbc.h" //getChainInPlace() and getChainInBuffer()
#include "chunk.h" //chunk type
#include "chunkQueue.h" //queue type and supporting functions
#include "debug.h" //pdebug()
#include "decrypt.h" //decryptInPlace() and decryptHeader()
#include "encrypt.h" //encryptInPlace() and encryptHeader()
#include "fileIO.h" //block read and write operations
#include "hash.h" //sf_hash()
#include "key.h" //handleKeys()
#include "mac.h" //checkHMAC() and genHMAC()
#include "noHash.h" //set_key()
#include "pad.h" //getNumBlocks()
#include "readThread.h"
#include "skeinApi.h" //SkeinCtx
#include "threefishApi.h" //ThreefishKey_t
#include "tfHeader.h" //checkHeader, genHeader() and stripIV()

#define SUCCESS 1
#define FILE_IO_FAIL 2
#define HEADER_CHECK_FAIL 3
#define MAC_CHECK_FAIL 4
#define QUEUE_OPERATION_FAIL 5
#define CIPHER_OPERATION_FAIL 6
#define MEMORY_ALLOCATION_FAIL 7
#define SIZE_CHECK_FAIL 8
#define MAC_GENERATION_FAIL 9

/******************
* Data structures *
*******************/

/************
* Functions *
*************/

int32_t runThreefizer(const arguments* args);

#endif
