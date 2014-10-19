#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "skeinApi.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct //arguments passed to argp containing argz (all non optional args)
{
     bool encrypt;
     bool free;
     bool hash;
     char* argz;
     size_t argz_len;
     SkeinSize_t state_size;
     uint8_t* password;
     uint64_t pw_length;
} arguments;

#endif
