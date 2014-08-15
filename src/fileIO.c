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
    bool status = true;
    FILE* my_write = fopen(fname, "wb");

    if(my_write == NULL)
    {
        status = false; 
        return status; 
    }

    fwrite(data, sizeof(uint8_t), data_size, my_write);
   
    if(ferror(my_write)) { status = false; }
    fclose(my_write);

    return status;
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

    uint64_t file_size = fseek(my_read, 0L, SEEK_END);
    rewind(my_read);

    uint8_t* data = calloc(file_size, sizeof(uint8_t));
    fread(data, sizeof(data), file_size, my_read);
    if(ferror(my_read)) 
    {
        perror("File Read Error unable to continue\n");
    }
    fclose(my_read);

    return data;
}

uint64_t getSize(const char* fname)
{
    struct stat st;
    stat(fname, &st);
    return st.st_size;
}
