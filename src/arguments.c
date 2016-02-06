#include "include/arguments.h"

void initArguments(arguments* arg)
{
    arg->key_file = NULL;
    arg->password = NULL;
    arg->rename_file = NULL;
    arg->target_file = NULL;
    arg->encrypt = true;
    arg->free = false;
    arg->hash = true;
    arg->hash_from_file = false;
    arg->rename = false;
    arg->legacy_hash = false;
    arg->threads_active = false;
    arg->state_size = Skein512;
    arg->file_size = 0;
    arg->pw_length = 0;
}
