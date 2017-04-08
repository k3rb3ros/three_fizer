#pragma once

#include <getopt.h> //get_opt_long()
#include <stdbool.h> //bool type 
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //memalloc(), free()
#include <string.h> //strcmp()
#include "arguments.h" //argument structure
#include "cbc.h" //cbc_decrypt, cbc_encrypt()
#include "commonConstants.h" //all relevant version and program strings
#include "controller.h" //runThreefizer()
#include "error.h" //error codes
#include "fileIO.h" //exists()
#include "progressBar.h" //calcOperations()
#include "util.h" //getSkeinSize()

#define LONG_OPTS_SIZE 11

//params for long options
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
