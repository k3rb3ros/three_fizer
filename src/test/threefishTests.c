#include "include/threefishTests.h"

void testSetupKey256()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish256, three_256_01_key, three_256_01_tweak);
    assert(tf_key.key[0] = three_256_01_key[0]);
    assert(tf_key.key[1] = three_256_01_key[1]);
    assert(tf_key.key[2] = three_256_01_key[2]);
    assert(tf_key.key[3] = three_256_01_key[3]);

    assert(tf_key.tweak[0] = three_256_01_tweak[0]);
    assert(tf_key.tweak[1] = three_256_01_tweak[1]); 

    assert(tf_key.stateSize = Threefish256);
}

void testSetupKey512()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish512, three_512_01_key, three_512_01_tweak);
    assert(tf_key.key[0] = three_512_01_key[0]);
    assert(tf_key.key[1] = three_512_01_key[1]);
    assert(tf_key.key[2] = three_512_01_key[2]);
    assert(tf_key.key[3] = three_512_01_key[3]);
    assert(tf_key.key[4] = three_512_01_key[4]);
    assert(tf_key.key[5] = three_512_01_key[5]);
    assert(tf_key.key[6] = three_512_01_key[6]);
    assert(tf_key.key[7] = three_512_01_key[7]);

    assert(tf_key.tweak[0] = three_512_01_tweak[0]);
    assert(tf_key.tweak[1] = three_512_01_tweak[1]);

    assert(tf_key.stateSize = Threefish512);
}

void testSetupKey1024()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish512, three_512_01_key, three_512_01_tweak);
    assert(tf_key.key[0] = three_1024_01_key[0]);
    assert(tf_key.key[1] = three_1024_01_key[1]);
    assert(tf_key.key[2] = three_1024_01_key[2]);
    assert(tf_key.key[3] = three_1024_01_key[3]);
    assert(tf_key.key[4] = three_1024_01_key[4]);
    assert(tf_key.key[5] = three_1024_01_key[5]);
    assert(tf_key.key[6] = three_1024_01_key[6]);
    assert(tf_key.key[7] = three_1024_01_key[7]);
    assert(tf_key.key[8] = three_1024_01_key[8]);
    assert(tf_key.key[9] = three_1024_01_key[9]);
    assert(tf_key.key[10] = three_1024_01_key[10]);
    assert(tf_key.key[11] = three_1024_01_key[11]);
    assert(tf_key.key[12] = three_1024_01_key[12]);
    assert(tf_key.key[13] = three_1024_01_key[13]);
    assert(tf_key.key[14] = three_1024_01_key[14]);
    assert(tf_key.key[15] = three_1024_01_key[15]);

    assert(tf_key.tweak[0] = three_1024_01_tweak[0]);
    assert(tf_key.tweak[1] = three_1024_01_tweak[1]);

    assert(tf_key.stateSize = Threefish1024);
}

void testThreefish256Encrypt()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish256, three_256_01_key, three_256_01_tweak);
    
    
}

void testThreefish512Encrypt()
{
}

void testThreefish1024Encrypt()
{
}

void testThreefish256Decrypt()
{
}

void testThreefish512Decrypt()
{
}

void testThreefish1024Decrypt()
{
}

void runThreefishTests()
{
    testSetupKey256();
    testSetupKey512();
    testSetupKey1024();
    testThreefish256Encrypt();
    testThreefish512Encrypt();
    testThreefish1024Encrypt();
    testThreefish256Decrypt();
    testThreefish512Decrypt();
    testThreefish1024Decrypt();
}

void Show08(uint64_t cnt, const uint8_t* b)
{
    for (uint64_t i=0;i < cnt;i++)
    {
        if (i %16 ==  0) printf(INDENT);
        else if (i % 4 == 0) printf(" ");
        printf(" %02X",b[i]);
        if (i %16 == 15 || i==cnt-1) printf("\n");
    }
}

