#ifndef THREEFIZER_H
#define THREEFIZER_H

#include <getopt.h>         //get_opt_long()
#include <stdbool.h>        //bool type
#include <stdint.h>         //uint types
#include <stdio.h>          //printf()
#include <stdlib.h>         //memalloc(), free()
#include <string.h>         //strcmp()
#include "arguments.h"      //argument structure
#include "cbc.h"            //cbc_decrypt, cbc_encrypt(
#include "controller.h"     //runThreefizer()
#include "error.h"          //error codes
#include "fileIO.h"         //exists()
#include "progressBar.h"    //calcOperations()
#include "util.h"           //getSkeinSize()

//Numerical constants
#define FNAME_BUFFER_SIZE 256

/**********************************
*  Constants and data structures  *
**********************************/

//program description
static const char* about = "A simple file encryption program that uses Threefish and Skein internally.\n"
                           "The intended use case is to encrypt files for backup locally \n"
                           "before uploading to potentiall insecure cloud backup services\n";

//program version
static const char* program_version = "Threefizer 0.4.3";

//support email
static const char* program_bug_address = "<M.or.riser.ryan+threefizer@gmail.com>";

static const char* usage = "Usage: threefizer [OPTION...] [FILE]...\n";

#define LONG_OPTS_SIZE 9

//params for long options
static const struct option long_options[] =
{
    { "blocksize", required_argument, 0, 'b' },
    { "decrypt", no_argument, 0, 'd' },
    { "encrypt", no_argument, 0, 'e' },
    { "help", no_argument, 0, 'h' },
    { "password", required_argument, 0, 'p' },
    { "passwordFile", required_argument, 0, 'P' },
    { "no-hash", no_argument, 0, 'n' },
    { "rename", required_argument, 0, 'r' },
    { "usage", no_argument, 0, 'u' },
    { "version", no_argument, 0, 'V' },
    { 0, 0, 0, 0} 
};

/**********************
* Function prototypes *
**********************/

#endif