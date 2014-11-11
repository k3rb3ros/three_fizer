#include "include/encrypt.h"

/*
* This function assumes padding and allocation has already been taken care of
*/
void encryptInPlace(ThreefishKey_t* key,
                           uint64_t* chain, 
                           uint64_t* plain_text, 
                           uint64_t num_blocks) 
{
    switch(key->stateSize) //call the corresponding decrypt and decrypt the header
    {
        case 256: cbc256Encrypt(key, chain, plain_text, num_blocks);
        break;
        case 512: cbc512Encrypt(key, chain, plain_text, num_blocks);
        break;
        case 1024: cbc1024Encrypt(key, chain, plain_text, num_blocks);
        break;
        default:
        {
            perror("Invalid state size cannot continue\n");
            exit(9);
        }
        break;
    }
}

