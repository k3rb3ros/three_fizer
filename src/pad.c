#include "include/pad.h"

//returns the size in Skein/Threefish state_size blocks the padded data will be
inline uint64_t getNumBlocks(const uint64_t input_length, const SkeinSize_t state_size)
{
    uint64_t blocks = 0;
    uint32_t bytes = 0;
    while(bytes < input_length)
    {
      bytes += (uint32_t) state_size/8;
      blocks++;
    }

    return blocks;
}

//returns the size in bytes the padded data will be
inline uint64_t getPadSize(const uint64_t input_length, const SkeinSize_t state_size)
{
    return (getNumBlocks(input_length, state_size)*(uint64_t)state_size)/8; 
}

uint64_t* pad(const uint8_t* input, const uint64_t input_length, const SkeinSize_t state_size)
{
     if(!input_length % state_size == 0) //Don't allocate any memory if the input text is already a padded size
     {
         uint64_t num_blocks = getNumBlocks(input_length, state_size);
         uint64_t* pad = calloc(num_blocks, state_size/sizeof(uint64_t)); //allocate zero filled memory for the padded input

         if(memcpy(pad, input, input_length) == NULL)//memcpy(pad, input, input_length) == NULL) //Copy input over into our padded buffer
         {
             perror("Error: Unable to allocate enough memory to pad input\n");
             free((void*)input); //free the old buffer
             return NULL; //return
         }
         free((void*)input); //free the old buffer
         return pad; //return the padded buffer
     }
     return (uint64_t*)input;
}
