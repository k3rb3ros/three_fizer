#ifndef KEY_H
#define KEY_H

#include "hash.h" //sf_hash()
#include "mac.h" //MacCtx_t type
#include "noHash.h" //noHash()
#include "scrypt.h" 
#include "tfHeader.h" //genIV()
#include "threefishApi.h" //ThreefishKey_t type
#include "tweak.h" //cipher_tweak constant

/* This takes the argument and hashed the user specified password into a key of block 
 * size or uses the user entered key directly if they have elected to use this option
 * the key is then hashed again and this is ued as the hmac key.
 * It assumes the user has correctly allocated block size buffers for the cipher and
 * mac key and passes them into the function
*/
bool handleKeys(arguments* args,
                ThreefishKey_t* cipher_context,
                MacCtx_t* mac_context);

#endif
