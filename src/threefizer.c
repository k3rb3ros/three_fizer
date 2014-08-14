/*
* By Ryan Morris Jul 2014
* threefizer command line file encryption/decryption utilitiy
*/

#include "include/threefizer.h"

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    arguments *a = state->input;
    switch(key)
    {
        case ARGP_KEY_ARG:
	{
            if(exists(arg))
            {
                argz_add(&a->argz, &a->argz_len, arg);
	    }
            else
            {
                 char msg[MAX_FILE_LENGTH] = { 0 }; 
                 sprintf(msg, "unable to open file: %s", arg);
                 argp_failure(state, 1, 0, msg);
            }
	}
        break;
	case ARGP_KEY_INIT:
        {
            a->free = false;
            a->encrypt = true;
            a->argz=NULL;
            a->argz_len = 0;
            a->skein_size = Skein512;
            a->password = NULL;
            a->pw_length = 0;
	}
        break;
        case ARGP_KEY_END:
        {
            size_t count = argz_count(a->argz, a->argz_len);
            if (count < 1) { argp_failure (state, 1, 1, "too few arguments"); }
        }
        break;
        case 'b': a->skein_size = getSkeinSize(arg);
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
                a->pw_length = strlen(arg);
            }
            else { 
                     argp_failure(state, 1, 2, "password is too short, try something less whimpy"); }
        }
        break;
        case 'P':
        {
            if(exists(arg))
            {
                a->free = true;
		a->password = readFile(arg);
                a->pw_length = getSize(arg); //write a function to get the length of a file
            }
            else
            {
                argp_failure(state, 1, 3, "Unable to read password file");
            }
        }
        break;
        default:
        break;
    }
    return 0;
}

int main(int argc, char*argv[])
{
    arguments arguments;
    int status = 0;
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
    status = argp_parse(&argp, argc, argv, 0, 0, &arguments);
    if(status == 0)
    {
      const char *prev = NULL;
      char* arg = 0;
      while((arg = argz_next(arguments.argz, arguments.argz_len, prev))) //parse argz 
      {
       prev = arg;
       if(exists(arg))
       {
           if (arguments.password == NULL) { arguments.password = askPassword(); }
           status = run_cipher(&arguments, arg);
           //preform the requested action on each file entered into the command line
       }
       else
       {
           fprintf(stderr, "Unable to open file %s for cipher operation\n", arg);
           status = 4;
       }
      }
      free(arguments.argz);
      if(arguments.free == true) { free(arguments.password); } //free password if we allocated it instead of taking it from argv
    }
    return status;
}
