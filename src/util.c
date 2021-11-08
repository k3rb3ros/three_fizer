#include "util.h"

//A sanity check for decrypting any files smaller then 4 blocks couldn't have been encrypted by this program
inline bool isGreaterThanThreeBlocks(const arguments* args)
{
    return args != NULL && (args->file_size > (uint64_t)((args->state_size/8) * 3));
}

/* same as clearBuff but inline */
static inline void zeroFill(void* buffer, const uint64_t length)
{
    uint8_t* ptr = buffer;
    for (uint64_t i=0; i<length; ++i)
    {
        ptr[i] = 0;
    }
}

inline bool validSize(const size_t size)
{
    return size == 256 || size == 512 || size == 1024;
}

SkeinSize_t getSkeinSize(const uint8_t* key)
{
    for (size_t i=0; i<N_BLOCK_LOOKUP; ++i)
    {
        const cipher_t* sym = &block_lookup[i];
        if (strcmp((char*)sym->key, (char*)key) == 0) { return sym->skein_size; }
    }

    return Skein512; //Should not happen
}

static inline uint8_t hexLookupNibble(uint8_t nibble)
{
    //this shouldn't happen
    if (nibble > N_HEX_LOOKUP) { return 0; }

    //lookup the hex value from the table
    return hex_lookup[nibble].hex;
}

uint8_t* binToHex(uint8_t* src, size_t size)
{
    //sanity checks
    if (src == NULL) { return NULL; }

    if (size == 0) 
    { 
        free(src);
	    return NULL;
    }

    //allocate storage for the hex buffer
    const uint64_t hex_size = 2*size;
    uint8_t* hex = calloc(hex_size+1, sizeof(uint8_t));

    for (uint64_t i=0; i<size; ++i)
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
    const static uint8_t pw_prompt[] = 
    { 
        'E', 'n', 't', 'e', 'r', ' ', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', ':', '\0'
    };
    const static uint8_t conf_prompt[] = 
    {
        'C', 'o', 'n', 'f', 'i', 'r', 'm', ' ', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd',
        ':', '\0'
    };

    uint8_t pw1[BUFF_SIZE] = { 0 };
    uint8_t pw2[BUFF_SIZE] = { 0 };
    uint16_t pw1_len = 0;
    uint16_t pw2_len = 0;
 
    while (match == false) //TODO find a portable way to do this
    {
        if (!first) { printf("Passwords do not match\n"); }

        first = false;

        pw1_len = getPassword((uint8_t*)pw_prompt, pw1, BUFF_SIZE, stdin);
        pw2_len = getPassword((uint8_t*)conf_prompt, pw2, BUFF_SIZE, stdin);

        if (pw1_len < 6 || pw2_len < 6) { printf("Password must be at least 6 characters in length\n"); } 
        else if ((pw1_len == pw2_len) && memcmp(pw1, pw2, pw2_len) == 0) { match = true; }
        else //clear buffers from failed pw attempts
        {
            zeroFill(pw1, BUFF_SIZE);
            zeroFill(pw2, BUFF_SIZE);
        }
    }

    printf("Password accepted\n");
    args->password = calloc(pw2_len+1, sizeof(uint8_t));
    memcpy(args->password, pw2, pw2_len);
    args->free = true; //set the flag to free it since we allocated memory for this pw
    args->pw_length = pw2_len; //ad the pw length to the arguments structure

    //zero_fill the temp pw_buffers
    zeroFill(pw1, BUFF_SIZE);
    zeroFill(pw2, BUFF_SIZE);
    pd3("password set to: %s\n", args->password);
}

ssize_t getPassword(uint8_t* prompt, uint8_t* line_buff, size_t buff_size, FILE* stream)
{
    struct termios old, new;
    int nread = -1;

    /* Display the prompt */
    if (prompt != NULL) { printf("%s\n", prompt); }

    /* Turn off echoing and fail if we can't */
    if (tcgetattr(fileno(stream), &old) != 0) { return nread; }

    new = old;
    new.c_lflag &= ~ECHO;

    if (tcsetattr(fileno(stream), TCSAFLUSH, &new) != 0) { return nread; }

    /* Read the password. */
    nread = getline((char**)&line_buff, &buff_size, stream);

    /* Strip out the carriage return */
    if(nread >= 1 && line_buff[nread -1] == '\n')
    {
        line_buff[nread-1] = 0;
        nread--;
    }

    /* turn echo back on */
    (void)tcsetattr(fileno(stream), TCSAFLUSH, &old);

    return nread;
}
