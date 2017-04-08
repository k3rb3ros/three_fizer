#include "include/fileIO.h"

inline bool isFile(const uint8_t* fname)
{
    struct stat st;
    stat((char*)fname, &st);
    return S_ISREG(st.st_mode);
}

inline bool exists(const uint8_t* fname)
{
    return access((char*)fname, R_OK) != -1;
}

bool writeBytes(const uint8_t* data,
                const size_t data_size,
                int_fast32_t write_fd)
{
    return (write(write_fd, data, data_size) == (ssize_t)data_size);
}

int_fast32_t openForRead(const uint8_t* fname)
{
    return open((char*)fname, O_RDONLY);
}

int_fast32_t openForWrite(const uint8_t* fname)
{
    //Write only and create file if it does not exist
    int flags = 0x0 | O_WRONLY | O_CREAT ;
    int mode = S_IRUSR | S_IWUSR; //user has read and write permission

    return open((char*)fname, flags, mode);
}

uint8_t* readBytes(const size_t data_size, int_fast32_t fd)
{
    if (fd < 0 || data_size == 0) { return NULL; }

    uint8_t* data = malloc(data_size);

    if (data == NULL) { return NULL; }

    if (read(fd, data, data_size) < (ssize_t)data_size)
    {
        free(data);
	    return NULL;
    }

    return data;
}

uint64_t getFileSize(const uint8_t* fname)
{
    struct stat st;
    return stat((char*)fname, &st) == 0 ? st.st_size : 0;
}
