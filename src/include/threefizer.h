#ifndef THREEFIZER_H
#define THREEFIZER_H

#include <argp.h> //argp_parse()
#include <argz.h> //argz
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //memalloc(), free()
#include <string.h> //strcmp()
#include "arguments.h" //argument structure
#include "cbc.h" //cbc_decrypt, cbc_encrypt(
#include "controller.h" //runThreefizer()
#include "fileIO.h" //exists()
#include "util.h" //getSkeinSize()

//Numerical constants
#define MAX_FILE_LENGTH 256

/*
***********************************
*  Constants and data structures  *
***********************************
*/

//program version
const char* argp_program_version = "Threefizer 0.3.0";

const char* argp_program_bug_address = "<M.or.riser.ryan@gmail.com>"; //support email

//program documentation
const static char doc[] = "A simple file encryption program that uses Threefish and Skein internally. The intended use case is to encrypt files for backup locally before upload to potentiall insecure cloud backup services\n";

//a description of the arguments we accept
const static char args_doc[] = "[FILE]...";

extern struct argp argp;

extern struct argp_option options[]; //Argp options

/*
************************
* Function prototypes
************************
*/

static error_t parse_opt (const int key, const char *arg, struct argp_state* state);

#endif 
