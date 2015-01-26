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
