#include "include/decrypt.h"

bool decryptHeader(ThreefishKey_t* key, uint64_t* header)
{
    pdebug("decryptHeader()\n");
    if(key == NULL || header == NULL) { return false; }    

    /*
    * The begining of all properly formed headers is the iv so we can pass the
    * header in as the IV as decryptInPlace knows how bug the IV block is and
    * will only use that data.
    *
    * We can the "stripIV" the pointer to point to only the data of the header
    * and use that as our data to decrypt.
    */
    uint64_t* header_data = stripIV(header, (uint64_t)key->stateSize);
    decryptInPlace(key, header, header_data, 1);
    return true;
}

/*
* This function assumes padding and allocation has already been taken care of
*/
void decryptInPlace(ThreefishKey_t* key,
                    const uint64_t* chain,
                    uint64_t* plain_text,
                    const uint64_t num_blocks)
{
    pd2("decryptInPlace(key:%lu, chain:%lu, plain_text:%lu, num_blocks:%lu)\n", key->key[0], chain[0], plain_text[0], num_blocks);
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
