#include "utilTests.h"

static void testIsGreaterThanThreeBlocks()
{
    printf("testIsGreaterThanThreeBlocks() ");

    arguments a1, a2, a3;

    a1.state_size = Skein256; a2.state_size = Skein256; a3.state_size = Skein256;
    a1.file_size = 0;
    a2.file_size = (Skein256/8)*3;
    a3.file_size = ((Skein256/8)*3)+1;
    assert(!isGreaterThanThreeBlocks(&a1));
    assert(!isGreaterThanThreeBlocks(&a2));
    assert(isGreaterThanThreeBlocks(&a3));

    a1.state_size = Skein512; a2.state_size = Skein512; a3.state_size = Skein512;
    a1.file_size = 0;
    a2.file_size = (Skein512/8)*3;
    a3.file_size = ((Skein512/8)*3)+1;
    assert(!isGreaterThanThreeBlocks(&a1));
    assert(!isGreaterThanThreeBlocks(&a2));
    assert(isGreaterThanThreeBlocks(&a3));

    a1.state_size = Skein1024; a2.state_size = Skein1024; a3.state_size = Skein1024;
    a1.file_size = 0;
    a2.file_size = (Skein1024/8)*3;
    a3.file_size = ((Skein1024/8)*3)+1;
    assert(!isGreaterThanThreeBlocks(&a1));
    assert(!isGreaterThanThreeBlocks(&a2));
    assert(isGreaterThanThreeBlocks(&a3));

    printf("passed\n");
}

static void testValidSize()
{
    printf("testValidSize() ");

    assert(validSize(Skein256));
    assert(validSize(Skein512));
    assert(validSize(Skein1024));
    assert(validSize(Threefish256));
    assert(validSize(Threefish512));
    assert(validSize(Threefish1024));

    assert(!validSize(0));
    assert(!validSize(224));
    assert(!validSize(128));

    printf("passed\n");
}

static void testGetSkeinSize()
{
    printf("testGetSkeinSize() ");
   
    uint8_t test_key[12] = { 0 };

    strcpy((char*)test_key, "");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "0");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "256");
    assert(getSkeinSize(test_key) == Skein256);
    strcpy((char*)test_key, "512");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "1024");
    assert(getSkeinSize(test_key) == Skein1024);
    strcpy((char*)test_key, "blarg");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "SAFE");
    assert(getSkeinSize(test_key) == Skein256);
    strcpy((char*)test_key, "Safe");
    assert(getSkeinSize(test_key) == Skein256);
    strcpy((char*)test_key, "safe");
    assert(getSkeinSize(test_key) == Skein256);
    strcpy((char*)test_key, "SECURE");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "Secure");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "secure");
    assert(getSkeinSize(test_key) == Skein512);
    strcpy((char*)test_key, "FUTUREPROOF");
    assert(getSkeinSize(test_key) == Skein1024);
    strcpy((char*)test_key, "FutureProof");
    assert(getSkeinSize(test_key) == Skein1024);
    strcpy((char*)test_key, "futureproof");
    assert(getSkeinSize(test_key) == Skein1024);

    printf("passed\n");
}

static void testBinToHex()
{
    printf("testBinToHex() ");
    
    assert(binToHex(NULL, SIZE_MAX) == NULL);
    assert(binToHex(NULL, 0) == NULL);

    uint8_t* test_bfr = calloc(45, sizeof(uint8_t));
    uint8_t exp_hex[90] = { 0 };
    strcpy((char*)&exp_hex, "54686520717569636B2062726F776E20666F78206A756D7073206F76657220746865206C617A7920646F6721");
    strcpy((char*)test_bfr, "The quick brown fox jumps over the lazy dog!");

    assert(strcmp((char*)test_bfr, "The quick brown fox jumps over the lazy dog!") == 0);
    test_bfr = binToHex(test_bfr, strlen((char*)test_bfr));
    assert(strcmp((char*)test_bfr, (char*)exp_hex) == 0);

    free(test_bfr);

    printf("passed\n");
}

//Can't automate test to getPassword() and askPassword() as they require user input from terminal

void runUtilTests()
{
    testIsGreaterThanThreeBlocks();
    testValidSize();
    testGetSkeinSize();
    testBinToHex();
}
