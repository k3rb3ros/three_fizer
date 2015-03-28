#include "util.h"

//A sanity check for decrypting any files smaller then 4 blocks couldn't have been encrypted by this program
inline bool isGreaterThanThreeBlocks(const arguments* args)
{
    return args != NULL && (args->file_size >= (uint64_t)(((args->state_size/8) * 3) + 1));
}

static inline void getLine(uint8_t* buffer, const uint64_t buffer_size) //get a line without the \n character from enter press
{
   uint8_t ch = 0;
   uint16_t count = 0;
   while((ch = getchar()) != '\n' && strlen((const char*)buffer) < buffer_size)
   {
       buffer[count++] = ch;
   }
}

static inline void zeroFill(void* buffer, const uint64_t length)
{
    uint8_t* ptr = buffer;
    for(uint64_t i=0; i<length; ++i)
    {
        ptr[i] = 0;
    }
}

inline bool validSize(const size_t size)
{
    return size == 256 || size == 512 || size == 1024;
}

SkeinSize_t getSkeinSize(const char* key)
{
    for(unsigned long i=0; i<N_BLOCK_LOOKUP; ++i)
    {
        key_t* sym = &block_lookup[i];
        if(strcmp(sym->key, key) ==0) { return sym->skein_size; }
    }
    return Skein512; //Should not happen
}

static inline uint8_t hexLookupNibble(uint8_t nibble)
{
    //this shouldn't happen
    if(nibble > N_HEX_LOOKUP) return 0;
    //lookup the hex value from the table
    return hex_lookup[nibble].hex;
}

uint8_t* binToHex(uint8_t* src, uint64_t size)
{
    //sanity checks
    if(src == NULL) { return NULL; }
    if(size == 0) 
    { 
        free(src);
	return NULL;
    }

    //allocate storage for the hex buffer
    const uint64_t hex_size = 2*size;
    uint8_t* hex = calloc(hex_size+1, sizeof(uint8_t));

    for(uint64_t i=0; i<size; ++i)
    {
	//split the byte into two nibble halves and look up their hex representation
        hex[(2*i)+0] = hexLookupNibble((src[i] & 0xf0) >> 4);
        hex[(2*i)+1] = hexLookupNibble(src[i] & 0x0f);
    }      

    //free the original buffer and return the hex
    free(src); 
    return hex;
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
 
    while(match == false) //TODO this should probably be updated with a newer way of doing this
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

        printf("\nEnter password: ");
        getLine((uint8_t*)pw1, BUFF_SIZE);
        printf("\nConfirm password: ");
        getLine((uint8_t*)pw2, BUFF_SIZE);

        /* restore terminal */
        if(tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
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

    printf("\nPassword accepted\n");
    pw_length = strlen(pw2);
    password = calloc(pw_length+1, sizeof(uint8_t));
    memcpy(password, pw2, pw_length);

    args->password = (uint8_t*)password; //add our pw to the arguments structurei
    args-> free = true; //set the flag to free it since we allocated memory for this pw
    args->pw_length = pw_length; //ad the pw length to the arguments structure
}
