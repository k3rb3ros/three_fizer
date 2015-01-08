#include "include/fileIOtests.h"

static void testFileExists()
{
    printf("testFileExists()");
    assert(exists("src/test/file_tests/simple_lines.dat") == true);
    assert(exists("src/test/file_teses/obviously_invalid_file") == false);
    printf(" passed\n");
}

static void testGetSize()
{
    printf("testGetSize()");       
    assert(getFileSize("src/test/file_tests/empty_file") == 0LU);
    assert(getFileSize("src/test/file_tests/null_file") == 0LU);
    assert(getFileSize("src/test/file_tests/simple_lines.dat") == 2160LU);
    printf(" passed\n");
}

static void testReadBlock()
{
    printf("testReadBlock()");
    int read = openForRead("src/test/file_tests/simple_lines.dat");
    assert(read > 0);

    uint64_t file_size = getFileSize("src/test/file_tests/simple_lines.dat");
    while(file_size > 0)
    {
        const uint8_t* line = readBytes(TEST_FILE_LINE_LENGTH, read);
	assert(strcmp((char*)line, REF_LINE) == 0);
	file_size -= TEST_FILE_LINE_LENGTH;
    }
    printf(" passed\n");
    close(read);
}

void runFileIOTests()
{
    testFileExists();
    testGetSize();
    testReadBlock();
}
