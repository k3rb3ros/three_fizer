#include "util.h"

SkeinSize_t getSkeinSize(char* key)
{
    for(unsigned long i=0; i<N_BLOCK_LOOKUP; ++i)
    {
        keyBlock_t* sym = &block_lookup[i];
        if(strcmp(sym->key, key) ==0) { return sym->skein_size; }
    }
    return NULL; //Should not happen
}

uint8_t* askPassword()
{
    bool first = true;
    bool match = false;
    char pw1[BUFF_SIZE] = {0};
    char pw2[BUFF_SIZE] = {0};
    char* password = NULL;
    struct termios oflags, nflags;
 
    while(match == false)
    {
        printf("\nEnter password:");
        if(!first) 
        {
            printf("Passwords do not match\n");
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
            return EXIT_FAILURE;
        }

        fgets(pw1, sizeof(pw1), stdin);
        printf("\nConfirm password:");
        fgets(pw2, sizeof(pw2), stdin);

        /* restore terminal */
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
            return EXIT_FAILURE;
        }

        if(strcmp(pw1, pw2) == 0 && strlen(pw2) > 6) { match = true; }
    }

    password = calloc(strlen(pw2), sizeof(char));
    strcpy(password, pw2); 
    return (uint8_t*)password;
}

void zeroFill(void* buffer, uint64_t length)
{
    uint8_t* ptr = buffer;
    for(uint64_t i=0; i<length; ++i)
    {
        ptr[i] = 0;
    }
}
