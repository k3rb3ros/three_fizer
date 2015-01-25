#include "include/fileIO.h"

inline bool exists(const uint8_t* fname)
{
    return access((char*)fname, R_OK) != -1;
}

bool writeBytes(const uint8_t* data, const uint64_t data_size, int64_t write_fd)
{
    int64_t status = write(write_fd, data, data_size);

    if(status < (int64_t)data_size) { return false; }
    if(fsync(write_fd) != 0) { return false; }

    return true;
}

inline int openForRead(const uint8_t* fname)
{
    return open((char*)fname, O_RDONLY);
}

inline int openForWrite(const uint8_t* fname)
{
    int flags = 0x0 | O_WRONLY | O_CREAT ; //Write only and create file if it does not exist
    int mode = S_IRUSR | S_IWUSR; //user has read and write permission 
    return open((char*)fname, flags, mode);
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

inline uint64_t getFileSize(const uint8_t* fname)
{
    struct stat st;
    return stat((char*)fname, &st) == 0 ? st.st_size : 0;
}
