/*
* By Ryan Morris Jul 2014
* threefizer command line file encryption/decryption utilitiy
*/

#include "include/cbc.h"
#include "include/threefizer.h"

int32_t lookup(char* key, kvp_t table[], int32_t size)
{
    for(int32_t i=0; i<size; ++i)
    {
        kvp_t* sym = &table[i];
	if (strcmp(sym->key, key) == 0) { return sym->val; }
    }	
    return BADARG;
}

int32_t parseArgs(int argc, int* count, char* argv[]) //Look for accepted arguments and try to parse them
{
    int32_t status = 1;
    switch (lookup(argv[*(count)], arguments, N_ARG_FLAGS))
    {	
        //Block size arguements
	case BLOCK_SIZE:
	if(*(count)+1 < argc) //If there is another arguement supplied then attempt to parse it
	{
	    block_size = parseBlockSize(argv[++(*count)]); //parse the block size
        }
	break;
	case DECRYPT: //TODO ask user for password to continue decrypt
	    cbcDecryptInPlace(block_size, password, "", 0);
	break;
	case ENCRYPT: //TODO ask user for password to continue encrypt
	    cbcEncryptInPlace(block_size, password, "", 0);
	break;
	case PW:
	if(*(count)+1 < argc) //If there is another arguement supplied then attempt to parse it
        {
            if(strlen(argv[++(*count)]) > 6)
            {
                password = (uint8_t*)argv[(*count)];
	        printf("\npassword accepted\n");
            }
            else
            {
                printf("Password not supplied or whimpy password entered\ntry something over 6 characters\n");
            }
        }
	break;
	case PW_FILE:
	    printf("Ready password file and convert it to key");
	break;
	case BADARG:
	    printf(USAGE);
	    status = -1;
	break;
	default :
	    printf(USAGE);
	    status = -1;
        break;
    }
	return status;
} 

uint32_t parseBlockSize(char* bs)
{
    uint32_t block_size = UINT32_MAX;
    switch (lookup(bs, block_sizes, N_BLOCK_SIZES))
    {
        case SAFE: block_size = 256;
	break;
	case SECURE: block_size = 512;
	break;
	case FUTURE_PROOF: block_size = 1024;
	break;
	case BADARG:
	default: block_size = 512;
	break;
    }
    return block_size;
}

int main(int argc, char*argv[])
{
    static int32_t count = 0;
    static int32_t ret_status = 0;	

    if (argc > 1)
    {
        while(++count<argc)
	{
	    if (parseArgs(argc, &count, argv) < 0)
	    {
	        ret_status = 2;
                break;
	    }
	}
    }
    else
    {
        printf(USAGE);
	ret_status = 1;
    }
    return ret_status;
}
