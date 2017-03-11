#ifndef THREEFIZER_H
#define THREEFIZER_H

#include <getopt.h> //get_opt_long()
#include <stdbool.h> //bool type 
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //memalloc(), free()
#include <string.h> //strcmp()
#include "arguments.h" //argument structure
#include "cbc.h" //cbc_decrypt, cbc_encrypt(
#include "controller.h" //runThreefizer()
#include "error.h" //error codes
#include "fileIO.h" //exists()
#include "progressBar.h" //calcOperations()
#include "util.h" //getSkeinSize()

//Numerical constants
#define FNAME_BUFFER_SIZE 256

/**********************************
*  Constants and data structures  *
**********************************/

//program description
const static uint8_t* about = (uint8_t*)"A simple file encryption program that uses Threefish and Skein internally. The intended use case is to encrypt files for backup locally before upload to potentially insecure cloud backup services\n";

//program version
const static uint8_t* program_version = (uint8_t*)"Threefizer 0.5.2";

//support email
const static uint8_t* program_bug_address = (uint8_t*)"k3rb3ros@k3rb3ros.ninja";

const static uint8_t* usage = (uint8_t*)"Usage: threefizer [OPTION...] [FILE]...\n";

//number of params in the struct below
#define LONG_OPTS_SIZE 11

//paramas for long options
const static struct option long_options[] = 
{
    { "blocksize", required_argument, 0, 'b' },
    { "decrypt", no_argument, 0, 'd' },
    { "encrypt", no_argument, 0, 'e' },
    { "help", no_argument, 0, 'h' },
    { "no_hash", no_argument, 0, 'n' },
    { "legacy-hash", no_argument, 0, 'l' },
    { "password", required_argument, 0, 'p' },
    { "password_file", required_argument, 0, 'P' },
    { "rename", required_argument, 0, 'r' },
    { "usage", no_argument, 0, 'u' },
    { "version", no_argument, 0, 'V' },
    { 0, 0, 0, 0} 
};

/**********************
* Function prototypes *
**********************/

#endif 
