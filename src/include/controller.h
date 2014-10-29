#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <arguments.h> //arguments structure
#include <stdint.h> //uint types
#include "chunk.h" //chunk types and queue types
#include "debug.h" //pdeubug()
#include "hash.h" //hashing sf_hash()
#include "noHash.h" //set_key()

int32_t runThreefizer(arguments* args);

#endif
