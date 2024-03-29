#include "fileIoTests.h"

static void testFileExists()
{
    printf("testFileExists()");
    assert(exists((uint8_t*)"tests/simple_lines.dat") == true);
    assert(exists((uint8_t*)"tests/obviously_invalid_file") == false);
    printf(" passed\n");
}

static void testGetFileSize()
{
    printf("testGetSize()");       
    assert(getFileSize((uint8_t*)"tests/empty_file") == 0LU);
    assert(getFileSize((uint8_t*)"tests/null_file") == 0LU);
    assert(getFileSize((uint8_t*)"tests/simple_lines.dat") == 2160LU);
    printf(" passed\n");
}

static void testReadBytes()
{
    printf("testReadBlock()");
    int read = openForRead((uint8_t*)"tests/simple_lines.dat");
    assert(read > 0);

    uint64_t file_size = getFileSize((uint8_t*)"tests/simple_lines.dat");
    while(file_size > 0)
    {
        uint8_t* line = readBytes(TEST_FILE_LINE_LENGTH, read);
	    assert(memcmp(line, REF_LINE, TEST_FILE_LINE_LENGTH) == 0);
	    file_size -= TEST_FILE_LINE_LENGTH;
        if(line != NULL) { free(line); }
    }
    printf(" passed\n");
    close(read);
}

static void testWriteBytes()
{
    printf("testWriteBytes()");
    const static uint64_t a_size = 27;
    const static uint8_t alphabet[] =
    { 
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'r', 'q', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\n'
    };

    int wr_fd = openForWrite((uint8_t*)"writeTest.dat");
    int re_fd = openForRead((uint8_t*)"writeTest.dat");
    assert(wr_fd > 0);
    assert(re_fd > 0);

    for(uint8_t i=0; i<26; ++i) //write 26 lines of the alphabet
    {
        assert(writeBytes(alphabet, a_size, wr_fd) == true); //write a line of the alphabet to the file

	    uint8_t* data = readBytes(a_size, re_fd); //read that line back from the file
	    assert(data != NULL);
	    for(uint8_t j=0; j<a_size; ++j) //check that the line we wrote is correct
	    {
	        assert(data[j] == alphabet[j]);
	    }
	    free(data);
    }

    assert(getFileSize((uint8_t*)"writeTest.dat") == 702); //check that the file size is correct
    printf(" passed\n");

    //close files
    close(wr_fd);
    close(re_fd);
}

void runFileIOTests()
{
    testFileExists();
    testGetFileSize();
    testReadBytes();
    testWriteBytes();
}
