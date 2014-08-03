#include "include/random.h"

uint8_t* getRand(uint64_t size)
{
    FILE* randFile = NULL;
    uint8_t* randomData = NULL;
    if(exists(HWRNG)) //try hardware random number generator first
    {
        randFile = openForBlockRead(HWRNG);
        if(randFile == NULL)
        {
            fprintf(stderr, "Unable to open %s cannot continue\n", HWRNG);
            return NULL; 
        }
    }
    else if(exists(PSRNG)) //use /dev/urandom since /dev/random blocks
    {
        randFile = openForBlockRead(PSRNG);
        if(randFile == NULL)
        {
            fprintf(stderr, "Unable to open %s cannot continue\n", PSRNG);
            return NULL; //consider exiting the program from here
        }
    }

    randomData = readBlock(size, randFile);
    fclose(randFile);

    return randomData;
}
