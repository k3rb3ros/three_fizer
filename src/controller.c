#include "include/controller.h"

#define SUCCESS 1

static uint64_t* handleKey(arguments* args)
{
    uint64_t* key = NULL;
    if(args->hash == true)
    {
       //hash the user entered password so the key matches state size
       key = (uint64_t*)sf_hash(args->password, args->pw_length, args->state_size);
    }
    else
    {
       //use the user entered password as the key directly
       key = set_key(args->password, args->pw_length, args->state_size);
    }

    return key;
}

int32_t run_threefizer(arguments* args)
{
    static int32_t status = SUCCESS;
    uint64_t* key = handleKey(args);

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %lu, password: [%s], pw_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length);

    if(key != NULL)
    {
        free(key);
    }

    return status;
}
