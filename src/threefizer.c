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

int parseArgs(int argc, int* count, char* argv[]) //Look for accepted arguments and try to parse them
{
	int status = 1;
	if(*(count) < argc)
	{
		switch (lookup(argv[*(count)], arguments, N_ARG_FLAGS))
		{	
			//Block size arguements
			case BLOCK_SIZE:
				if(*(count)+1 < argc) //If there is another arguement supplied then attempt to parse it
				{
					parseBlockSize(argv[++(*count)]); //parse the block size
				}
			break;
			case DECRYPT:
				cbc_decrypt();
			break;
			case ENCRYPT:
				cbc_encrypt();
			break;
			case PW:
				printf("Read password and convert it to key");
			break;
			case PW_FILE:
				printf("Ready password file and convert it to key");
			break;
			case BADARG:
				printf(USAGE);
				status = -1;
			break;
		}
	}
	return status;
} 

int parseBlockSize(char* bs)
{
	int block_size = -1;
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
	int count = 1;
	static int ret_status = 0;	

	if(argc > 0) //parse all command line arguements
	{
		for(count; count<argc; ++count)
		{
			if (parseArgs(argc, &count, argv) < 0)
			{
				break;
			}
		}
	}

	return ret_status;
}
