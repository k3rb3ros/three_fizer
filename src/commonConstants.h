#pragma once

#include <stdint.h>

/*
 * This header file contains all commain constants used to alter the behavior and display of the
 * program.
 */

#define FNAME_BUFFER_SIZE 256
#define HASH_BUFFER_SIZE 5120000 //50MB
#define QUE_SIZE 10 //10*50MB*3Queues = 1.5GB max queue usage
#define MAX_CHUNK_SIZE 51200000 //50MB (whatever this is it must be a multiple of 64)

#define INDENT "    "

//program description
const static uint8_t* about = (uint8_t*)"A simple file encryption program that uses Threefish and Skein internally. The intended use case is to encrypt files for backup locally before upload to potentially insecure cloud backup services.\n";

//program version
const static uint8_t* program_version = (uint8_t*)"Threefizer 0.6.0";

//support email
const static uint8_t* program_bug_address = (uint8_t*)"k3rb3ros@k3rb3ros.ninja";

//usage message
const static uint8_t* usage = (uint8_t*)"Usage: threefizer [OPTION...] [FILE]...\n";
