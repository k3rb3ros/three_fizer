/*
* By Ryan Morris
* threefizer command line file encryption/decryption utilitiy
*/

#include "include/threefizer.h"

void printOptions()
{
    printf("OPTIONS\n");
    for(int i=0; i< LONG_OPTS_SIZE; ++i)
    {
        printf("-%c, --%s", long_options[i].val, long_options[i].name);
        if(long_options[i].has_arg == required_argument)
        { 
            printf(" "); 
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
                case 'r':
                    printf("NewFileName");
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
    bool mand_arg = false;
    bool parsing = true;
    arguments arguments;
    int arg = 0;
    int option_index = 0; 
    int status = 0;

    extern char *optarg;
    extern int optind, opterr, optopt;

    initArguments(&arguments);

    while ((arg = getopt_long(argc, argv, "b:dehnp:P:r:uV", long_options, &option_index)) != -1
           && parsing
          )
    {
        switch(arg)
        {
            case 'b':
                arguments.state_size = getSkeinSize(optarg);
            break;
            case 'd':
                arguments.encrypt = false;
            break;
            case 'e':
                if(!arguments.encrypt)
                {
                    perror("Both encrypt and decrypt flag are set\n");
                    parsing = false;
                    status = ARG_PARSING_ERROR;
                }
            break;
            case 'h':
                printf("%s", usage);
                printf("\n%s\n", about);
                printOptions();
                printf("\nMandatory or optional arguments to long options are also mandatory or optional for any corresponding short options.\n");
                printf("\nReport bugs to %s\n", program_bug_address);
                parsing = false;
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
                if(exists((uint8_t*)optarg) && isFile((uint8_t*)optarg))
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
            case 'r':
                arguments.rename = true;
                arguments.rename_file = (uint8_t*)optarg; 
            break;
            case 'u':
                printf("%s", usage);
                parsing = false;
            break;
            case 'V':
                printf("%s", program_version);
                parsing = false;
            break;
            default:
                status = ARG_PARSING_ERROR;
            break;
        }
    }

    if(parsing)
    {
        //there should always be a file name specified as the last argument
        if(optind < argc)
        {
            if(exists((uint8_t*)argv[argc-1]) && isFile((uint8_t*)argv[argc-1]))
            {
                arguments.target_file = (uint8_t*)argv[argc-1];
                arguments.file_size = getFileSize((uint8_t*)argv[argc-1]);
                mand_arg = true;
            }
            else
            {
                status = INVALID_TARGET_FILE; 
            }
        }
        else { status = ARG_PARSING_ERROR; }
    }
 
    if(status == 0 && mand_arg)
    {
        //ask the user password if one was not specified
        if(!arguments.hash_from_file && arguments.password == NULL)
        { 
            askPassword(&arguments); 
        }

        //perform the requested action on each file entered into the command line
        status = runThreefizer(&arguments);
    }

    if(status != 0) { printError(status); }
    else { printf("\nOperation succeeded"); }

    if(arguments.free == true) { free(arguments.password); } //free password if we allocated it instead of taking it from argv

    return status;
}
