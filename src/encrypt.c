#include "include/encrypt.h"

//encrypt a properly formatted header
bool encryptHeader(ThreefishKey_t* key,
                   const uint64_t* header)
{
    if(key == NULL || header == NULL) { return false; }
    
    uint64_t* header_data = stripIV(header, key->stateSize); //get a pointer to the header w/out iv
    encryptInPlace(key, header, header_data, 1);

    return true;
}

/*
* This function assumes padding and allocation has already been taken care of
*/
void encryptInPlace(ThreefishKey_t* key,
                    const uint64_t* chain, 
                    uint64_t* plain_text, 
                    const uint64_t num_blocks) 
{
    switch(key->stateSize) //call the corresponding cbc Encrypt function
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
