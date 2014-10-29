#ifndef THREEFIZER_H
#define THREEFIZER_H

#include <arguments.h> //argument structure
#include <argp.h> //argp_parse()
#include <argz.h> //argz
#include <cbc.h> //cbc_decrypt, cbc_encrypt()
#include <fileIO.h> //exists()
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //memalloc(), free()
#include <string.h> //strcmp()
//#include "cipher.h" //run_cipher
#include "util.h" //getSkeinSize()

//Numerical constants
#define MAX_FILE_LENGTH 256

/*
**********************************
*  Constants and data structures
**********************************
*/

//program version
const char* argp_program_version = "Threefizer 0.3.0";

const char* argp_program_bug_address = "<M.or.riser.ryan@gmail.com>"; //support email

extern struct argp argp;

//program documentation
const static char doc[] = "A simple file encryption program that uses Threefish and Skein internally\n";

//a description of the arguments we accept
const static char args_doc[] = "[FILE]...";

extern struct argp_option options[]; //Argp options

/*
************************
* Function prototypes
************************
*/

static error_t parse_opt (int key, char *arg, struct argp_state* state);

#endif 
