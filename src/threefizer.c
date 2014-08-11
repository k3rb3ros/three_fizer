/*
* By Ryan Morris Jul 2014
* threefizer command line file encryption/decryption utilitiy
*/

#include "include/cbc.h"
#include "include/threefizer.h"

int lookup(char* key, kvp_t table[], int size)
{
    for(int i=0; i<size; ++i)
    {
        kvp_t* sym = &table[i];
	if (strcmp(sym->key, key) == 0) { return sym->val; }
    }	
    return BADARG;
}

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    arguments *a = state->input;
    switch(key)
    {
        case ARGP_KEY_ARG:
	{
            argz_add(&a->argz, &a->argz_len, arg);
            printf("adding argument {%s}", arg);
	}
        break;
	case ARGP_KEY_INIT:
        {
            a->encrypt = true;
            a->argz=NULL;
            a->block_size = SECURE;
            a->argz_len = 0;
            a->password = NULL;
	}
        break;
        case ARGP_KEY_END:
        {
            size_t count = argz_count(a->argz, a->argz_len);
            if (count < 1) { argp_failure (state, 1, 0, "too few arguments"); }
        }
        break;
        case 'b': a->block_size = parseBlockSize(arg);
	break;
        case 'd': a->encrypt = false;
        break;
        case 'e': a->encrypt = true;
        break;
        case 'p':
        {
            if(strlen(arg) > 6)
            {
                a->password = (uint8_t *)arg;
            }
            else { argp_failure(state, 1, 1, "password is too short"); }
        }
        break;
        case 'P':
        {
        }
        break;
        default:
        break;
    }
    return 0;
}

int parseBlockSize(char* bs)
{
    int _block_size = lookup(bs, block_sizes, N_BLOCK_SIZES);
    if(_block_size == BADARG) return SECURE;
    return _block_size;
}

int main(int argc, char*argv[])
{
    struct argp_option options[] = 
    {
        { 0, 0, 0, 0, "Functional Options (encrypt is default)", 1 },
        { "decrypt", 'd', 0, 0, "Decrypt the specified file(s)" },
        { "encrypt", 'e', 0, 0, "Encrypt the specified file(s)" },
        { 0, 0, 0, 0, "Property Options", 2 },
        { "blocksize", 'b', "BlockSize", 0, "Specify the block size of the cipher and hash" },
        { "password", 'p', "Password", 0, "Specify a password from the command line" },
        { "passwordfile", 'P', "PasswordFile", 0, "Specify a password file" },
        { 0 }
    };
    struct argp argp = { options, parse_opt, args_doc, doc };
    arguments arguments;
    if(argp_parse(&argp, argc, argv, 0, 0, &arguments) ==0)
    {
      const char *prev = NULL;
      char* arg = 0;
      while((arg = argz_next(arguments.argz, arguments.argz_len, prev)))
      {
       prev = arg;
      }
      free(arguments.argz);
      //if(arguments.password != NULL) { free(arguments.password); }
    }
    return 0;
}
