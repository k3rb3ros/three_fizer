#include "include/error.h"

void printError(int error)
{
    if(error == 0) { return; }

    fprintf(stderr, "Error\n");

    switch(error)
    {
	case CIPHER_OPERATION_FAIL:
	     fprintf(stderr, "Cipher operation failure\n");
	break;
	case FILE_IO_FAIL:
	     fprintf(stderr, "File I/O failure ensure disk is not full and you have write access to the file you are trying to encrypt\n");
	break;
        case HEADER_CHECK_FAIL:
             fprintf(stderr, "Header check failure. Ensure that the file you are trying to decrypt was created with this program, the password is correct and that the block size is the same as what it was when encrypting. If you are still getting this error then the file has been corrupted or modifiend since encryption.\n", stderr );
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
	case QUEUE_OPERATION_FAIL:
	     fprintf(stderr, "Error queing resources operation stopped to prevent deadlock.\n");
	break;
	
        default: break;	
    }

    fprintf(stderr, "Cannot continue\n");
}
