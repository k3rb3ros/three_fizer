/*
* By Ryan Morris
* threefizer command line file encryption/decryption utilitiy
*/

#include "include/threefizer.h"

/*static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    arguments* a = state->input;
    switch(key)
    {
        pdebug("Parse_opt %c\n" , key);
        case ARGP_KEY_ARG:
	{
            if(exists(arg))
            {
                argz_add(&a->argz, &a->argz_len, arg);
                a->file_size = getFileSize(a->argz);
	    }
            else
            {
                 char msg[FNAME_BUFFER_SIZE] = { 0 }; 
                 sprintf(msg, "unable to open file: %s", arg);
                 argp_failure(state, 1, 0, msg);
            }
	}
        break;
	case ARGP_KEY_INIT:
        {
            a->free = false;
            a->encrypt = true;
	    a->hash_from_file = false;
            a->hash = true;
            a->argz= NULL;
            a->argz_len = 0;
            a->state_size = Skein512;
	    a->key_file = NULL;
            a->password = NULL;
            a->pw_length = 0;
            a->file_size = 0;
	}
        break;
        case ARGP_KEY_END:
        {
            const size_t count = argz_count(a->argz, a->argz_len);
            if (count < 1) { argp_failure (state, 1, TOO_FEW_ARGUMENTS, "too few arguments"); }
        }
        break;
        case 'b': a->state_size = getSkeinSize(arg);
	break;
        case 'd': a->encrypt = false;
        break;
        case 'e': a->encrypt = true;
        break;
        case 'n': a->hash = false;
        break;
        case 'p':
        {
            if(strlen(arg) > 6)
            {
                a->password = (uint8_t*)arg;
                a->pw_length = strlen(arg);
            }
            else { argp_failure(state, 1, PASSWORD_TOO_SHORT, "password is too short, try something less whimpy"); }
        }
        break;
        case 'P':
        {
            if(exists(arg))
            {
                a->free = true;
		a->hash_from_file = true;
		a->key_file = (uint8_t*)arg;
            }
            else
            {
                argp_failure(state, 1, PASSWORD_FILE_READ_FAIL, "Unable to read password file");
            }
        }
        break;
        default:
        break;
    }
    return 0;
}*/

void printOptions()
{
    printf("OPTIONS\n");
    for(int i=0; i< LONG_OPTS_SIZE; ++i)
    {
        printf("-%c, --%s", long_options[i].val, long_options[i].name);
        if(long_options[i].has_arg == required_argument)
        { 
            printf("="); 
            int c = long_options[i].val;
            switch(c)
            {
                case 'b':
                    printf("BlockSize");
                break;
                case 'p':
                    printf("Password");
                break;
                case 'P':
                    printf("Passwordfile");
                break;
                default:
                break;
            }
        }
        printf("\n");
    }
}

int main(int argc, char*argv[])
{
    bool parsing = true;
    arguments arguments;
    int arg = 0;
    int option_index = 0; 
    int status = 0;

    initArguments(&arguments);

    while ((arg = getopt_long(argc, argv, "b:dehnp:P:uV", long_options, &option_index)) != -1
           && parsing
          )
    {
        switch(arg)
        {
            case 'b':
                arguments.state_size = getSkeinSize(optarg);
            break;
            case 'd':
                arguments.encrypt = true;
            break;
            case 'e':
                if(!arguments.encrypt)
                {
                    perror("Both encrypt and decrypt flag are set\n");
                    parsing = false;
                    status = ARG_PARSING_ERROR;
                }
            break;
            case 'n':
                arguments.hash = false;
            break;
            case 'p':
                if(strlen(optarg) > 6)
                {
                    arguments.password = (uint8_t*)optarg;
                    arguments.pw_length = strlen(optarg);
                }
                else 
                {
                    parsing = false; 
                    status = PASSWORD_TOO_SHORT;
                }
            break;
            case 'P':
                if(exists(optarg))
                {
                    arguments.free = true;
                    arguments.hash_from_file = true;
                    arguments.key_file = (uint8_t*)optarg;
                }
                else
                {
                    parsing = false;
                    status = INVALID_PASSWORD_FILE;
                }
            break;
            case 'u':
                printf("%s", usage);
                parsing = false;
            break;
            case 'V':
                printf("%s", program_version);
                parsing = false;
            break;
            case '?':
                printf("%s", usage);
                printf("\n%s\n", about);
                printOptions();
                printf("\nMandatory or optional arguments to long options are also mandatory or optional for any corresponding short options.\n");
                printf("\nReport bugs to %s\n", program_bug_address);
                parsing = false;

            break;
            default:
                status = ARG_PARSING_ERROR;
            break;
        }
    }
 
    if(status == 0)
    {
        /*
        if(exists(arg))
        {
            if (arguments.password == NULL && arguments.key_file == NULL)
            { 
                askPassword(&arguments); 
            }

            //perform the requested action on each file entered into the command line
            status = runThreefizer(&arguments);
	    if(status != 0) { printError(status); }
	    else { printf("Operation succeeded"); }
        }
        else
        {
            fprintf(stderr, "Unable to open file %s for cipher operation\n", arg);
            status = 4;
        }*/
    }
    //free(arguments.argz);
    if(arguments.free == true) { free(arguments.password); } //free password if we allocated it instead of taking it from argv

    return status;
}
