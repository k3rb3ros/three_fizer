#include "include/fileIO.h"

inline bool exists(const char* fname)
{
    return access(fname, R_OK) != -1;
}

bool writeBlock(const uint8_t* data, const uint64_t data_size, FILE* write)
{
    if(ferror(write)) { return false; }
    fwrite(data, sizeof(uint8_t), data_size, write);
    if(ferror(write)) { return false; }

    return true;
}

bool writeBytes(const uint8_t* data, const uint64_t data_size, int64_t write_fd)
{
    int64_t status = write(write_fd, data, data_size);

    if(status < (int64_t)data_size) { return false; }
    if(fsync(write_fd) != 0) { return false; }

    return true;
}

bool writeEntireFile(const char* fname, uint8_t* data, uint64_t data_size)
{
   int64_t fd = open(fname, O_WRONLY);

   if(fd < 0) { return false; }
   if(write(fd, data , data_size) < (int64_t)data_size) { return false; }
   if(fsync(fd) != 0) { return false; }

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

inline FILE* openForBlockRead(const char* fname)
{
    return fopen(fname, "rb");
}

inline FILE* openForBlockWrite(const char* fname)
{
    return fopen(fname, "wb+");
}

inline int openForRead(const char* fname)
{
    return open(fname, O_RDONLY);
}

inline int openForWrite(const char* fname)
{
    return open(fname, O_WRONLY);
}

uint8_t* readBytes(const uint64_t data_size, int64_t fd)
{
    if(fd < 0 || data_size == 0) { return NULL; }

    uint8_t* data = calloc(data_size, sizeof(uint8_t));
    if(data == NULL) { return 0; }

    if(read(fd, data, data_size) < (int64_t)data_size)
    {
        free(data);
	return NULL;
    }

    return data;
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

uint8_t* readEntireFile(const char* fname)
{
    int64_t fd = open(fname, O_RDONLY);
    if(fd < 0) { return NULL; }

    uint64_t file_size = getFileSize(fname);
    if(file_size == 0) { return NULL; }

    uint8_t* data = calloc(file_size, sizeof(uint8_t));
    if(data == NULL) { return NULL; }

    //read the file
    if(read(fd, data, file_size) < (int64_t)file_size)
    {
        free(data);
	return NULL;
    }
    
    if(fsync(fd) != 0) { return false; }

    return data;
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

inline uint64_t getFileSize(const char* fname)
{
    struct stat st;
    return stat(fname, &st) == 0 ? st.st_size : 0;
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
