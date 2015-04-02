#include <string.h>

#include "include/error.h"

void printError(int error)
{
	char message[2048] = "";

    if(error == 0) { return; }

    fprintf(stderr, "\nError\n");

    switch(error)
    {
        case ARG_PARSING_ERROR:
            fprintf(stderr, "Error parsing arguments!\n");
        	break;
		case CIPHER_OPERATION_FAIL:
			fprintf(stderr, "Cipher operation failure!\n");
			break;
		case FILE_IO_FAIL:
			strcat(message, "File I/O failure!\n");
			strcat(message, "Ensure disk is not full and you have write access to the file you are trying to encrypt.\n");
			fprintf(stderr, message);
			break;
		case HEADER_CHECK_FAIL:
			strcat(message, "Header check failure!\n");
			strcat(message, "Ensure that the file you are trying to decrypt was created with this program,\n");
			strcat(message, "the password is correct and that the block size is the same as what it was when encrypting.\n");
			strcat(message, "If you are still getting this error then the file has been corrupted or modified since encryption.\n");
			fprintf(stderr, message);
			break;
		case INVALID_PASSWORD_FILE:
			fprintf(stderr, "The password file specified is invalid\n");
			break;
		case INVALID_TARGET_FILE:
			strcat(message, "The target file specified is not a valid file.\n");
			strcat(message, "Ensure that it exists you have access to it and that it is a regular file.\n");
			fprintf(stderr, message);
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
			break;
        default:
			strcat(message, "Something went wrong\n");
			strcat(message, "but the programmer was too lazy to write a detailed error message for this case.\n");
			strcat(message, "Please report bug!\n");
			fprintf(stderr, message);
			break;
    }

    fprintf(stderr, "Cannot continue\n");
}
