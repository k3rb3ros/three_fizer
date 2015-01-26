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
