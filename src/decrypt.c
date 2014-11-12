#include "include/decrypt.h"

/*
* This function assumes padding and allocation has already been taken care of
*/
void decryptInPlace(const ThreefishKey_t* key,
                    const uint64_t* chain,
                    const uint64_t* plain_text,
                    const uint64_t num_blocks)
{
    switch(key->stateSize) //call the corresponding cbc Encrypt function
    {
        case 256: cbc256Decrypt(key, chain, plain_text, num_blocks);
        break;
        case 512: cbc512Decrypt(key, chain, plain_text, num_blocks);
        break;
        case 1024: cbc1024Decrypt(key, chain, plain_text, num_blocks);
        break;
        default:
        {
            perror("Invalid state size cannot continue\n");
            exit(9);
        }
        break;
    }
}
