#include "util.h"

SkeinSize_t getSkeinSize(char* key)
{
    for(unsigned long i=0; i<N_BLOCK_LOOKUP; ++i)
    {
        keyBlock_t* sym = &block_lookup[i];
        if(strcmp(sym->key, key) ==0) { return sym->skein_size; }
    }
    return Skein512; //Should not happen
}

void askPassword(arguments* args)
{
    bool first = true;
    bool match = false;
    char pw1[BUFF_SIZE] = {0};
    char pw2[BUFF_SIZE] = {0};
    char* password = NULL;
    int pw_length = 0;
    struct termios oflags, nflags;
 
    while(match == false)
    {
        if(!first) 
        {
            printf("\nPasswords do not match");
            zeroFill(pw1, BUFF_SIZE);
            zeroFill(pw2, BUFF_SIZE);
        }
        first = false;

        /* disabling echo */
        tcgetattr(fileno(stdin), &oflags);
        nflags = oflags;
        nflags.c_lflag &= ~ECHO;
        nflags.c_lflag |= ECHONL;

        if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        {
            perror("tcsetattr");
            exit(6);
        }

        printf("\nEnter password:");
        fgets(pw1, sizeof(pw1), stdin);
        printf("\nConfirm password:");
        fgets(pw2, sizeof(pw2), stdin);

        /* restore terminal */
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
            exit(6);
        }

	if(strlen(pw1) > BUFF_SIZE) exit(7); //If buffer overflow occurs force exit

        if(strlen(pw1) < 6)
	{
            printf("\nPassword must be at least 6 characters in length");
        } 
        else if(strcmp(pw1, pw2) == 0) { match = true; }
    }

    printf("\nPassword accepted");
    pw_length = strlen(pw2);
    password = calloc(pw_length+1, sizeof(uint8_t));
    memcpy(password, pw2, pw_length);

    args->password = (uint8_t*)password; //add our pw to the arguments structurei
    args-> free = true; //set the flag to free it since we allocated memory for this pw
    args->pw_length = pw_length; //ad the pw length to the arguments structure
}

void zeroFill(void* buffer, uint64_t length)
{
    uint8_t* ptr = buffer;
    for(uint64_t i=0; i<length; ++i)
    {
        ptr[i] = 0;
    }
}
