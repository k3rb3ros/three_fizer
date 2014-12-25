#include "include/fileIO.h"

inline bool exists(const char* fname)
{
    return access(fname, R_OK) != -1;
}

inline FILE* openForBlockRead(const char* fname)
{
    return fopen(fname, "rb");
}

inline FILE* openForBlockWrite(const char* fname)
{
    return fopen(fname, "wb+");
}

uint8_t* readBlock(const uint64_t data_size, FILE* read)
{
    pdebug("readBlock()\n");
    if(ferror(read))
    {
        fclose(read);
        perror("Error reading block\n"); 
        return NULL;
    } 

    uint8_t* data = calloc(data_size, sizeof(uint8_t));
    const uint64_t size = fread(data, sizeof(uint8_t), data_size, read);

    if(ferror(read))
    {
        fclose(read);
        perror("Error reading block\n"); 
        return NULL; 
    }
    
    if(size == data_size)
    {
        return data;
    }

    perror("Unable to read requested number of bytes\n"); 
    free(data);
    return NULL;    
}

uint8_t* readFile(const char* fname)
{
    FILE* my_read = fopen(fname, "rb");

    if(my_read == NULL || !fseek(my_read, 0L, SEEK_END) == 0) { return NULL; };
    
    const uint64_t file_size = ftell(my_read);
    rewind(my_read);
    uint8_t* data = calloc(file_size+1, sizeof(uint8_t));
    fread(data, sizeof(uint8_t), file_size, my_read);
    if(ferror(my_read)) 
    {
        perror("File Read Error unable to continue\n");
    }
    fclose(my_read);

    return data;
}

bool writeBlock(const uint8_t* data, const uint64_t data_size, FILE* write)
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

    pdebug("(writeFile) fname:%s, data:%x, data_size:%lu\n", (char*)fname, (unsigned int)data, data_size);
    fwrite(data, sizeof(uint8_t), data_size, my_write);
   
    if(ferror(my_write)) { status = false; }
    fclose(my_write);

    return status;
}

inline uint64_t getSize(const char* fname)
{
    struct stat st;
    stat(fname, &st);
    return st.st_size;
}

inline void terminateFile(FILE* write) //write the terminating \n and close the File
{
    const static uint8_t eol[1] = { '\n' };
    if(!ferror(write)) 
    {
        fwrite(eol, sizeof(uint8_t), 1, write);
        fclose(write);
    }
}
