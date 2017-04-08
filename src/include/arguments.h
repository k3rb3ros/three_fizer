#pragma once

#include <stdbool.h> //bool type
#include <stdint.h> //uint_t types
#include "skein3fish/skeinApi.h" //SkeinSize_t type

typedef struct //arguments passed to argp containing argz (all non optional args)
{
     uint8_t* key_file;
     uint8_t* password;
     uint8_t* rename_file;
     uint8_t* target_file;
     uint64_t* iv;
     size_t file_size;
     size_t pw_length;
     SkeinSize_t state_size;
     bool encrypt : 1;
     bool free : 1;
     bool hash : 1;
     bool hash_from_file : 1;
     bool legacy_hash : 1;
     bool rename : 1;
     bool threads_active : 1;
} arguments;

void initArguments(arguments* arg);
