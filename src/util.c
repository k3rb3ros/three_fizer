#include "util.h"

//A sanity check for decrypting any files smaller then 4 blocks couldn't have been encrypted by this program
inline bool isGreaterThanThreeBlocks(const arguments* args)
{
    return args != NULL && (args->file_size >= (uint64_t)(((args->state_size/8) * 3) + 1));
}

/*static inline void getLine(uint8_t* buffer, const uint64_t buffer_size) //get a line without the \n character from enter press
{
   uint8_t ch = 0;
   uint16_t count = 0;
   while((ch = getchar()) != '\n' && strlen((const char*)buffer) < buffer_size)
   {
       buffer[count++] = ch;
   }
}*/

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
        const key_t* sym = &block_lookup[i];
        if(strcmp(sym->key, key) == 0) { return sym->skein_size; }
    }

    return Skein512; //Should not happen
}

static inline uint8_t hexLookupNibble(uint8_t nibble)
{
    //this shouldn't happen
    if(nibble > N_HEX_LOOKUP) { return 0; }

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
    const char pw_prompt[] = 
    { 
        'E', 'n', 't', 'e', 'r', ' ', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', ':', '\0'    };
    const char conf_prompt[] = 
    {
        'C', 'o', 'n', 'f', 'i', 'r', 'm', ' ', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd',
        ':', '\0'
    };
    char* buff = NULL;
    char* pw1 = NULL; //[BUFF_SIZE] = {0};
    char* pw2 = NULL; //[BUFF_SIZE] = {0};
    char* password = NULL;
    int pw_length = 0;
 
    while(match == false) //TODO this should probably be updated with a newer way of doing this
    {
        if(!first) 
        {
            printf("\nPasswords do not match\n");

            //clear pw buffs before they are freed to prevent leaking what the user types
            zeroFill(pw1, strlen(pw1));
            zeroFill(pw2, strlen(pw2));
            free(pw1);
            pw1 = NULL;
            free(pw2);
            pw2 = NULL;
        }

        first = false;

        buff = (char*)getpass(&pw_prompt);
        pw1 = calloc(strlen(buff), sizeof(char));

        if(pw1 == NULL)
        {
           printf("Unable to allocate memory for password\n");
           exit(MEMORY_ALLOCATION_FAIL); 
        }
        memcpy(pw1, buff, strlen(buff));

        buff = (char*)getpass(&conf_prompt);
        pw2 = calloc(strlen(buff), sizeof(char));

        if(pw2 == NULL)
        {
            printf("Unable to allocate memory for password\n");
            exit(MEMORY_ALLOCATION_FAIL); 
        }
        memcpy(pw2, buff, strlen(buff));

        if(strlen(pw1) < 6)
        {
            printf("\nPassword must be at least 6 characters in length\n");
        } 
        else if(strcmp(pw1, pw2) == 0) { match = true; }
    }

    printf("\nPassword accepted\n");
    pw_length = strlen(pw2);
    password = calloc(pw_length+1, sizeof(uint8_t));
    memcpy(password, pw2, pw_length);

    args->password = (uint8_t*)password; //add our pw to the arguments structure
    args-> free = true; //set the flag to free it since we allocated memory for this pw
    args->pw_length = pw_length; //ad the pw length to the arguments structure
}
