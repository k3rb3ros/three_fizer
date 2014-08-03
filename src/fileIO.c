#include "include/fileIO.h"

bool exists(const char* fname)
{
    return access(fname, R_OK) != -1;
}

FILE* openForBlockRead(const char* fname)
{
    return fopen(fname, "rb");
}

FILE* openForBlockWrite(const char* fname)
{
    return fopen(fname, "ab");
}

bool writeBlock(uint8_t* data, uint64_t data_size, FILE* write)
{
    if(ferror(write)) { return false; }
    
    fwrite(data, sizeof(data), data_size, write);
    fclose(write);

    if(ferror(write)) { return false; }

    return true;
}

bool writeFile(const char* fname, uint8_t* data, uint64_t data_size)
{
    FILE* my_write = fopen(fname, "wb");

    if(my_write == NULL) { return false; }

    fwrite(data, sizeof(uint8_t), data_size, write);
    fclose(write);
   
    if(ferror(my_write)) { return false; }

    return true;
}

uint8_t* readBlock(uint64_t data_size, FILE* read)
{
    if(ferror(read)) { return NULL; }

    uint8_t* data = calloc(data_size, sizeof(uint8_t));
    fread(data, sizeof(uint8_t), data_size, read);

    if(ferror(read))
    {
        fclose(read); 
        return NULL; 
    }

    return data;    
}

uint8_t* readFile(const char* fname)
{
    FILE* my_read = fopen(fname, "rb");

    if(my_read == NULL) { return NULL; };

    fseek(my_read, 0L, SEEK_END);
    uint64_t file_size = (my_read);
    rewind(my_read);

    uint8_t* data = calloc(file_size, sizeof(uint8_t));
    fread(data, sizeof(data), file_size, my_read);
    fclose(my_read);

    if(ferror(my_read)) { return data; }

    return data;
}
