/*
* By Ryan Morris Jul 2014
* threefizer command line file encryption/decryption utilitiy
*/

#include "include/threefizer.h"

#define BADARG -1 //Bad arguement

//Valid arguement types
#define BLOCK_SIZE 1
#define DECRYPT 2
#define ENCRYPT 3
#define PW 4
#define PW_FILE 5

#define SAFE 1
#define SECURE 2
#define FUTURE_PROOF 3

#define USAGE "Usage: threefizer <options> filename"

typedef struct //A kvp structure
{ 
	char *key; 
	int val;
} kvp_t;

static kvp_t arguments[] = //a lookup table of argument types
{
	{"-bs", BLOCK_SIZE}, {"-d", DECRYPT}, {"-e", ENCRYPT}, {"-p", PW}, {"-pf", PW_FILE}
};

static kvp_t block_sizes[] = //a lookup table of cipher block sizes
{
	{ "256", SAFE }, { "512", SECURE }, { "1024", FUTURE_PROOF },
	{ "SAFE", SAFE }, { "SECURE", SECURE }, { "FUTURE_PROOF", FUTURE_PROOF } 	
};

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
		switch (lookup(argv[*(count)], arguments, 5))
		{	
			//Block size arguements
			case BLOCK_SIZE: 
				printf("Block Size");
			break;
			case DECRYPT:
				printf("DECRYPT");
			break;
			case ENCRYPT:
				printf("ENCRYPT");
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
