#include "include/threefishTests.h"

//Test that setting up a 256 bit key works
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

//Test that setting up a 512 bit key works
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

//test that setting up a 1024 bit key works
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

//test that 256bits null encrypted matches the test vector
void testThreefish256NullEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_256[SAFE_SIZE] = { 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish256, three_256_00_key, three_256_00_tweak);
    threefishEncryptBlockWords(&tf_key, three_256_00_plain_text, test_block_256); 
    assert(test_block_256[0] == three_256_00_expected_result[0]);
    assert(test_block_256[1] == three_256_00_expected_result[1]);
    assert(test_block_256[2] == three_256_00_expected_result[2]);
    assert(test_block_256[3] == three_256_00_expected_result[3]);
}

//test that the 256 non null encrypted with a non null tweak match the test vector 
void testThreefish256TVEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_256[SAFE_SIZE] = { 0L, 0L, 0L, 0L };
   
    threefishSetKey(&tf_key, Threefish256, three_256_01_key, three_256_01_tweak);
    threefishEncryptBlockWords(&tf_key, three_256_01_plain_text, test_block_256);

    /* plaintext feed forward */
    for (uint64_t i = 0; i < SAFE_SIZE; i++) { test_block_256[i] ^= three_256_01_plain_text[i]; }

    printf("threefish256TestVectorEncrypt()\n");

    printf("Actual: \n");
    Show08(Threefish256/8, (uint8_t *)test_block_256);
    printf("\n Expected: \n");
    Show08(Threefish256/8, (uint8_t *)three_256_01_expected_result);

    assert(test_block_256[0] == three_256_01_expected_result[0]);
    assert(test_block_256[1] == three_256_01_expected_result[1]);
    assert(test_block_256[2] == three_256_01_expected_result[2]);
    assert(test_block_256[3] == three_256_01_expected_result[3]);
}

void testThreefish512NullEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_512[SECURE_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish512, three_512_00_key, three_512_00_tweak);
    threefishEncryptBlockWords(&tf_key, three_512_00_plain_text, test_block_512); 
    assert(test_block_512[0] == three_512_00_expected_result[0]);
    assert(test_block_512[1] == three_512_00_expected_result[1]);
    assert(test_block_512[2] == three_512_00_expected_result[2]);
    assert(test_block_512[3] == three_512_00_expected_result[3]);
    assert(test_block_512[4] == three_512_00_expected_result[4]);
    assert(test_block_512[5] == three_512_00_expected_result[5]);
    assert(test_block_512[6] == three_512_00_expected_result[6]);
    assert(test_block_512[7] == three_512_00_expected_result[7]);
}

//test that the 512 non null encrypted with a non null tweak match the test vector 
void testThreefish512TVEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_512[SECURE_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
   
    threefishSetKey(&tf_key, Threefish512, three_512_01_key, three_512_01_tweak);
    threefishEncryptBlockWords(&tf_key, three_512_01_plain_text, test_block_512);
    
    /* plaintext feed forward */
    for (uint64_t i = 0; i < SECURE_SIZE; i++) { test_block_512[i] ^= three_512_01_plain_text[i]; }

    printf("threefish512TestVectorEncrypt()\n");

    printf("Actual: \n");
    Show08(Threefish512/8, (uint8_t *)test_block_512);
    printf("\n Expected: \n");
    Show08(Threefish512/8, (uint8_t *)three_512_01_expected_result);

    assert(test_block_512[0] == three_512_01_expected_result[0]);
    assert(test_block_512[1] == three_512_01_expected_result[1]);
    assert(test_block_512[2] == three_512_01_expected_result[2]);
    assert(test_block_512[3] == three_512_01_expected_result[3]);
    assert(test_block_512[4] == three_512_01_expected_result[4]);
    assert(test_block_512[5] == three_512_01_expected_result[5]);
    assert(test_block_512[6] == three_512_01_expected_result[6]);
    assert(test_block_512[7] == three_512_01_expected_result[7]);
}

//test that 1024 bits null encrypted matches the test vector
void testThreefish1024NullEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_1024[FP_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish1024, three_1024_00_key, three_1024_00_tweak);
    threefishEncryptBlockWords(&tf_key, three_1024_00_plain_text, test_block_1024); 
    assert(test_block_1024[0] == three_1024_00_expected_result[0]);
    assert(test_block_1024[1] == three_1024_00_expected_result[1]);
    assert(test_block_1024[2] == three_1024_00_expected_result[2]);
    assert(test_block_1024[3] == three_1024_00_expected_result[3]);
    assert(test_block_1024[4] == three_1024_00_expected_result[4]);
    assert(test_block_1024[5] == three_1024_00_expected_result[5]);
    assert(test_block_1024[6] == three_1024_00_expected_result[6]);
    assert(test_block_1024[7] == three_1024_00_expected_result[7]);
    assert(test_block_1024[8] == three_1024_00_expected_result[8]);
    assert(test_block_1024[9] == three_1024_00_expected_result[9]);
    assert(test_block_1024[10] == three_1024_00_expected_result[10]);
    assert(test_block_1024[11] == three_1024_00_expected_result[11]);
    assert(test_block_1024[12] == three_1024_00_expected_result[12]);
    assert(test_block_1024[13] == three_1024_00_expected_result[13]);
    assert(test_block_1024[14] == three_1024_00_expected_result[14]);
    assert(test_block_1024[15] == three_1024_00_expected_result[15]);
}

//test that the 512 non null encrypted with a non null tweak match the test vector 
void testThreefish1024TVEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_1024[FP_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish1024, three_1024_01_key, three_1024_01_tweak);
    threefishEncryptBlockWords(&tf_key, three_1024_01_plain_text, test_block_1024);

    /* plaintext feed forward */
    for (uint64_t i = 0; i < FP_SIZE; i++) { test_block_1024[i] ^= three_1024_01_plain_text[i]; }

    printf("threefish1024TestVectorEncrypt()\n");

    printf("Actual: \n");
    Show08(Threefish1024/8, (uint8_t *)test_block_1024);
    printf("\n Expected: \n");
    Show08(Threefish1024/8, (uint8_t *)three_1024_01_expected_result);

    assert(test_block_1024[0] == three_1024_01_expected_result[0]);
    assert(test_block_1024[1] == three_1024_01_expected_result[1]);
    assert(test_block_1024[2] == three_1024_01_expected_result[2]);
    assert(test_block_1024[3] == three_1024_01_expected_result[3]);
    assert(test_block_1024[4] == three_1024_01_expected_result[4]);
    assert(test_block_1024[5] == three_1024_01_expected_result[5]);
    assert(test_block_1024[6] == three_1024_01_expected_result[6]);
    assert(test_block_1024[7] == three_1024_01_expected_result[7]);
    assert(test_block_1024[8] == three_1024_01_expected_result[8]);
    assert(test_block_1024[9] == three_1024_01_expected_result[9]);
    assert(test_block_1024[10] == three_1024_01_expected_result[10]);
    assert(test_block_1024[11] == three_1024_01_expected_result[11]);
    assert(test_block_1024[12] == three_1024_01_expected_result[12]);
    assert(test_block_1024[13] == three_1024_01_expected_result[13]);
    assert(test_block_1024[14] == three_1024_01_expected_result[14]);
    assert(test_block_1024[15] == three_1024_01_expected_result[15]);
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
    testThreefish256NullEncrypt();
    testThreefish256TVEncrypt();
    testThreefish512NullEncrypt();
    testThreefish512TVEncrypt();
    testThreefish1024NullEncrypt();
    testThreefish1024TVEncrypt();
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

