#include "include/pad.h"

inline uint64_t getNumBlocks(uint64_t input_length, SkeinSize_t state_size)
{
    uint64_t blocks = 0;
    uint64_t bytes = 0;
    while(bytes < input_length)
    {
      bytes += (uint64_t) state_size/8;
      blocks++;
    }

    return blocks;
}

inline uint64_t getPadSize(uint64_t input_length, SkeinSize_t state_size)
{
    uint64_t padSize = input_length;

    while(padSize % state_size != 0) 
    { padSize++; }
    return padSize/8;  
}

uint64_t* pad(uint8_t* input, uint64_t input_length, SkeinSize_t state_size)
{
     if(!input_length % state_size == 0) //Don't allocate any memory if the input text is already a padded size
     {
         uint64_t num_blocks = getNumBlocks(input_length, state_size);
         uint64_t* pad = calloc(num_blocks, state_size/sizeof(uint64_t)); //allocate zero filled memory for the padded input

         if(memcpy(pad, input, input_length) == NULL)//memcpy(pad, input, input_length) == NULL) //Copy input over into our padded buffer
         {
             perror("Error: Unable to allocate enough memory to pad input\n");
             free(input); //free the old buffer
             return NULL; //return
         }
         free(input); //free the old buffer
         return pad; //return the padded buffer
     }
     return (uint64_t*)input;
}
