#pragma once

#include <stdbool.h> //bool type
#include <stdint.h> //stdint types
#include "arguments.h" //arguments type
#include "chunk.h" //chunk type and support functions
#include "chunkQueue.h" //queue type and support functions
#include "skein3fish/skeinApi.h" 

typedef struct
{
    SkeinCtx_t skein_context;
    SkeinCtx_t* skein_context_ptr;
    uint8_t out_action;
    uint32_t key_size;
    uint64_t* mac_key;
    uint64_t digest_byte_size;
} MacCtx_t;

bool checkMAC(chunk* expected, const uint8_t* generated, uint64_t mac_byte_size);

uint64_t* genMAC(MacCtx_t* context, 
                 queue* in, 
                 queue* out);

void InitMacCtx(const arguments* args,
                MacCtx_t* context,
                uint64_t* mac_key);
