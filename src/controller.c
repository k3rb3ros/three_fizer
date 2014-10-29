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
       key = noHashKey(args->password, args->pw_length, args->state_size);
    }

    return key;
}

static bool queueFile(arguments* args, queue* readQueue)
{

}

int32_t runThreefizer(arguments* args)
{
    queue* readQueue = createQueue(QUE_SIZE);
    static int32_t status = SUCCESS;
    static ThreefishKey_t tf_key;
    uint64_t* key = handleKey(args); //generate the key

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length);

    threefishSetKey(&tf_key, (ThreefishSize_t)args->state_size, key, tf_tweak);

    if(key != NULL)
    {
        free(key);
    }
    if(readQueue != NULL)
    {
        free(readQueue->elements);
        free(readQueue);
    }

    return status;
}
