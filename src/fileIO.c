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
    return fopen(fname, "wb");
}

bool writeBlock(uint8_t* data, uint64_t data_size, FILE* write)
{
    if(ferror(write)) { return false; }
    fwrite(data, sizeof(uint8_t), data_size, write);
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
    uint64_t file_size = 0;

    if(my_read == NULL || !fseek(my_read, 0L, SEEK_END) == 0) { return NULL; };
    
    file_size = ftell(my_read);
    rewind(my_read);

    uint8_t* data = calloc(file_size+1, sizeof(uint8_t));
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
