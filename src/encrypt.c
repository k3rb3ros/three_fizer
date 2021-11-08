#include "encrypt.h"

//encrypt a properly formatted header
bool encryptHeader(ThreefishKey_t* key,
                   const uint64_t* header)
{
    if(key == NULL || header == NULL) { return false; }

    //get a pointer to the data part of the header    
    uint64_t* header_data = stripIV(header, key->stateSize);
    encryptInPlace(key, header, header_data, 1);

    return true;
}
