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

bool decryptHeader(const ThreefishKey_t* key,
                   const uint64_t* header)
{
    pdebug("decryptHeader()\n");
    if(key == NULL || header == NULL) { return false; }    

    /*The begining of all properly formed headers is the iv so we can pass the
    * header in as the IV as decryptInPlace knows how bug the IV block is and
    * will only use that data.
    *
    * We can the "stripIV" the pointer to point to only the data of the header
    * and use that as our data to decrypt.
    */
    uint64_t* header_data = stripIV(key, header);
    decryptInPlace(key, header, header_data, 1);
}

