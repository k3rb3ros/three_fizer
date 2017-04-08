#pragma once

#include "arguments.h" //arguments struct
#include "debug.h" //pdebug()
#include "error.h" //error codes
#include "skein3fish/skeinApi.h" //SkeinSize_t
#include "skein3fish/threefishApi.h" //ThreefishKey_t
#include <stdbool.h> //bool type
#include <stddef.h> //size_t, ssize_t
#include <stdint.h> //uint8_t type
#include <stdlib.h> //free()
#include <stdio.h> //printf()
#include <string.h> //strcmp()
#include <termios.h> //tcsetattr()
#include <unistd.h> //getpass()

//Invalid argument type
#define BADARG -1

#define BUFF_SIZE 1024

//Skein/Threefish block sizes
#define SAFE 256
#define SECURE 512
#define FUTURE_PROOF 1024

//Compile time size of our lookup table
#define N_BLOCK_LOOKUP sizeof(block_lookup)/sizeof(cipher_t)
#define N_HEX_LOOKUP 15

typedef struct
{
    uint8_t binary;
    uint8_t hex;
} hex_t;

typedef struct 
{
    uint8_t* key;
    SkeinSize_t skein_size;
} cipher_t;

//a lookup table containing skein/threefish cipher sizes. Note these are just for parsing block size input not cryptographic operations
const static cipher_t block_lookup[] =
{
    { (uint8_t*)"256", Skein256 }, { (uint8_t*)"512", Skein512 }, { (uint8_t*)"1024", Skein1024 },
    { (uint8_t*)"SAFE", Skein256 }, { (uint8_t*)"SECURE", Skein512 }, { (uint8_t*)"FUTUREPROOF", Skein1024 },
    { (uint8_t*)"safe", Skein256 }, { (uint8_t*)"secure", Skein512 }, { (uint8_t*)"futureproof", Skein1024 },
    { (uint8_t*)"Safe", Skein256 }, { (uint8_t*)"Secure", Skein512 }, { (uint8_t*)"FutureProof", Skein1024 },
    { (uint8_t*)"FUTURE_PROOF", Skein1024 }, { (uint8_t*)"future_proof", Skein1024 }, { (uint8_t*)"Future_Proof", Skein1024 }
};

//a lookup table for converting binary nibbles(4 bits) to hex
const static hex_t hex_lookup[] =
{
    { 0, '0' }, { 1, '1' }, { 2, '2' }, { 3, '3' }, 
    { 4, '4' }, { 5, '5' }, { 6, '6' }, { 7, '7' }, 
    { 8, '8' }, { 9, '9' }, { 10, 'A' }, { 11, 'B' }, 
    { 12, 'C' }, { 13, 'D' }, { 14, 'E' }, { 15, 'F' }, 
};

bool isGreaterThanThreeBlocks(const arguments* args);

bool validSize(const size_t size);

SkeinSize_t getSkeinSize(const uint8_t* key);

uint8_t* binToHex(uint8_t* src, uint64_t size);

//Turns off console echo so the user can safely type a password
ssize_t getPassword(uint8_t* prompt, uint8_t** lineptr, size_t n, FILE *stream);

void askPassword(arguments* args);
