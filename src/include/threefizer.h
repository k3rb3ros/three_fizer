#ifndef THREEFIZER_H
#define THREEFIZER_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

//Invalid arguement type
#define BADARG -1
//Valid arguement types
#define BLOCK_SIZE 1
#define DECRYPT 2
#define ENCRYPT 3
#define PW 4
#define PW_FILE 5

//Block size types
#define SAFE 1
#define SECURE 2
#define FUTURE_PROOF 3

//Constant Strings
#define USAGE "Usage: threefizer <options> filename"

//Numerical constants
#define N_ARG_FLAGS sizeof(arguments)/sizeof(kvp_t)
#define N_BLOCK_SIZES sizeof(block_sizes)/sizeof(kvp_t)

static int block_size = 512;

typedef struct //A kvp structure
{
        char *key;
        int val;
} kvp_t;

static kvp_t arguments[] = //a lookup table of argument types
{
        { "-bs", BLOCK_SIZE }, { "-d", DECRYPT }, { "-e", ENCRYPT }, { "-p", PW }, { "-pf", PW_FILE }
};

static kvp_t block_sizes[] = //a lookup table of cipher block sizes
{
        { "256", SAFE }, { "512", SECURE }, { "1024", FUTURE_PROOF },
        { "SAFE", SAFE }, { "SECURE", SECURE }, { "FUTURE_PROOF", FUTURE_PROOF }
};

int lookup(char* key, kvp_t table[], int size); //Looks up the value for a KVP by Key Value

int parseArgs(int argc, int* count, char* argv[]);

int parseBlockSize(char* bs); //An overglorified way of using a switch to parse strings

#endif 
