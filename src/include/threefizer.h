#ifndef THREEFIZER_H
#define THREEFIZER_H

#include <argp.h> //argp_parse()
#include <argz.h> //argz
#include <stdint.h> //uint types
#include <stdio.h> //printf()
#include <stdlib.h> //memalloc(). free()
#include <string.h> //strcmp(), 

//Invalid arguement type
#define BADARG -1

//Block size types
#define SAFE 256
#define SECURE 512
#define FUTURE_PROOF 1024

//Numerical constants
#define N_BLOCK_SIZES sizeof(block_sizes)/sizeof(kvp_t)

/*
**********************************
*  Constants and data structures
**********************************
*/

//program version
const char* argp_program_version = "threefizer 0.2";

const char* argp_program_bug_address = "<M.or.riser.ryan@gmail.com>"; //support email

extern struct argp argp;

//program documentation
const static char doc[] = "A simple file encryption program that uses Threefish and Skein internally\n";

//a description of the arguments we accept
const static char args_doc[] = "[FILE]...";

extern struct argp_option options[]; //Argp options

typedef struct //A kvp structure used to speed up string to int conversion
{
    char *key;
    int val;
} kvp_t;

typedef struct //arguements passed to argp and containing argz
{
    bool encrypt;
    char* argz;
    int block_size;
    size_t argz_len;
    uint8_t* password;
} arguments;

static kvp_t block_sizes[] = //a lookup table of cipher block sizes
{
        { "256", SAFE }, { "512", SECURE }, { "1024", FUTURE_PROOF },
        { "SAFE", SAFE }, { "SECURE", SECURE }, { "FUTURE_PROOF", FUTURE_PROOF }
};

static error_t parse_opt (int key, char *arg, struct argp_state* state);

/*
************************
* Function prototypes
************************
*/

int lookup(char* key, kvp_t table[], int32_t size); //Looks up the value for a KVP by Key Value

int parseBlockSize(char* bs);

#endif 
