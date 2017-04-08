#include "include/random.h"

uint8_t* getRand(const size_t size)
{
    //pdebug("(getRand)\n");
    uint8_t* random_data = NULL;

    if (size > 0)
    {
        //allocate memory the random output
        random_data = malloc(size);
        pcg32_random_t rng;

        //Sead the RNG with the system time and address of function from stdio
        pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)sscanf, time(NULL) ^ (intptr_t)printf);

        //call the rng for each byte of randomness (this is overkill)
        for (size_t s=0; s<size; ++s)
        {
            random_data[s] = (uint8_t)pcg32_random_r(&rng);
        }
    }

    return random_data;
}
