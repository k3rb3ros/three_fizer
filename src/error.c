#include "include/error.h"

void printError(int error)
{
    if(error == 0) { return; }

    fprintf(stderr, "Error\n");

    switch(error)
    {
        case ARG_PARSING_ERROR:
            fprintf(stderr, "Error parsing arguments\n");
        break;
	case CIPHER_OPERATION_FAIL:
	    fprintf(stderr, "Cipher operation failure\n");
	break;
	case FILE_IO_FAIL:
	    fprintf(stderr, "File I/O failure ensure disk is not full and you have write access to the file you are trying to encrypt\n");
	break;
        case HEADER_CHECK_FAIL:
            fprintf(stderr, "Header check failure. Ensure that the file you are trying to decrypt was created with this program, the password is correct and that the block size is the same as what it was when encrypting. If you are still getting this error then the file has been corrupted or modified since encryption.\n", stderr );
	break;
        case INVALID_PASSWORD_FILE:
            fprintf(stderr, "The password file specified is invalid\n");
        break;
	case KEY_GENERATION_FAIL:
	    fprintf(stderr, "Key generation failed\n");
	break;
	case MAC_CHECK_FAIL:
	    fprintf(stderr, "MAC check failure the encrypted file has been modified or corrupted\n");
	break;
	case MAC_GENERATION_FAIL:
	    fprintf(stderr, "Error generating MAC\n");
	break;
	case MEMORY_ALLOCATION_FAIL:
	    fprintf(stderr, "Error allocating memory\n");
	break;
        case PASSWORD_TOO_SHORT:
            fprintf(stderr, "The password entered is too short\n");
        break;
	case QUEUE_OPERATION_FAIL:
	    fprintf(stderr, "Queing resources operation stopped to prevent deadlock.\n");
	break;
        case TOO_FEW_ARGUMENTS:
            fprintf(stderr, "Too few arguments specified\n");	
        default:
	    fprintf(stderr, "Something went wrong but the programmer was too lazy to write a detailed error message for this case. Sorry you are SOL.\n");
	break;	
    }

    fprintf(stderr, "Cannot continue\n");
}
