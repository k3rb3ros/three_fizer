#include "cipher.h"

cipher initCipher(uint8_t* filename, uint8_t* password, SkeinSize_t skein_size)
{
    cipher cipher_internal;
    
    cipher_internal.file = openForBlockRead((const char*)filename); //this is going to differ between encrypting and decrypting
    cipher_internal.key = hash(password, strlen((char*)password), skein_size);
    cipher_internal.padded_input = 0; 

    return cipher_internal;
}
