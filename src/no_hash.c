#include "include/no_hash.h"

uint64_t* set_key(uint8_t* password, uint64_t length, uint64_t state_size)
{
    uint64_t* key = NULL;    

    if(length < state_size)
    {
        key = calloc(length, sizeof(uint8_t));
        printf("You have turned off password hashing and entered a password or password file that is smaller then the state size. A loss of cryptographic strength of your data may occur\n");
    }

    if(length >= state_size)
    {
        key = calloc(state_size/8, sizeof(uint8_t));
        length = state_size/8;
        printf("You have turned off password hashing and entered a password that is larger then the state size. Only the first %lu characters of your password will be used as the key\n", state_size/8);
    }

    memcpy(key, password, length);

    return key;
}
