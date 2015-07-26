#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <arguments.h> //arguments structure
#include <limits.h> //ULLONG_MAX
#include <pthread.h> //pthread_t
#include <stdint.h> //uint types
#include <stdio.h> //rename()
#include <unistd.h> //unlink()
#include "cbc.h" //getChainInPlace() and getChainInBuffer()
#include "chunk.h" //chunk type
#include "chunkQueue.h" //queue type and supporting functions
#include "cryptoThread.h" //decryptQueue(), encryptQueue(), setUpCryptParams()
#include "debug.h" //pdebug()
#include "decrypt.h" //decryptInPlace() and decryptHeader()
#include "encrypt.h" //encryptInPlace() and encryptHeader()
#include "error.h" //error codes
#include "hash.h" //sf_hash()
#include "key.h" //handleKeys()
#include "mac.h" //checkHMAC() and genHMAC()
#include "macThread.h" //setUpMacParams()
#include "noHash.h" //set_key()
#include "pad.h" //getNumBlocks()
#include "readThread.h" //setUpReadParams()
#include "skeinApi.h" //SkeinCtx
#include "threefishApi.h" //ThreefishKey_t
#include "tfHeader.h" //checkHeader, genHeader() and stripIV()
#include "writeThread.h" //asyncWrite()

#define THREEFIZER_SUCCESS 0
#define TEMP_FILE_NAME_SIZE 10 //size in bytes of the temp file

/************
* Functions *
*************/

int runThreefizer(arguments* args);

#endif
