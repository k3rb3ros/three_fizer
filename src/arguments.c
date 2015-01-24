#include "include/arguments.h"

void initArguments(arguments* arg)
{
    arg->free = false;
    arg->encrypt = true;
    arg->hash_from_file = false;
    arg->hash = true;
    arg->argz= NULL;
    arg->argz_len = 0;
    arg->state_size = Skein512;
    arg->key_file = NULL;
    arg->password = NULL;
    arg->pw_length = 0;
    arg->file_size = 0;
}
