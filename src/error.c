#include "include/error.h"

void printError(int error)
{
    if(error == 0) { return; }
    switch(error)
    {
	case CIPHER_OPERATION_FAIL:
	     perror("Cipher operation failure\n");
	break;
	case FILE_IO_FAIL:
	      perror("File I/O failure ensure disk is not full and you have write access to the file you are trying to encrypt\n");
	break;
        case HEADER_CHECK_FAIL:
             perror("Header check failure. Ensure that the file you are trying to decrypt was created with this program, the password is correct and that the block size is the same as what it was when encrypting. If you are still getting this error then the file has been corrupted or modifiend since encryption.\n" );
	break;
	case MAC_CHECK_FAIL:
	      perror("MAC check failure the encrypted file has been modified or corrupted\n");
	break;
	case MAC_GENERATION_FAIL:
	      perror("Error generating MAC\n");
	break;
	case MEMORY_ALLOCATION_FAIL:
	      perror("Error allocating memory\n");
	break;
	case QUEUE_OPERATION_FAIL:
	      perror("Error queing resources operation stopped to prevent deadlock.\n");
	break;
	
        default: break;	
    }

    perror("Cannot continue\n");
}
