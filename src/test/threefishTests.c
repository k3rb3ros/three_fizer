#include "include/threefishTests.h"

//Test that setting up a 256 bit key works
static void testSetupKey256()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish256, (uint64_t*)three_256_01_key, (uint64_t*)three_256_01_tweak);
    printf("testSetupKey256()");
    assert(tf_key.key[0] == three_256_01_key[0]);
    assert(tf_key.key[1] == three_256_01_key[1]);
    assert(tf_key.key[2] == three_256_01_key[2]);
    assert(tf_key.key[3] == three_256_01_key[3]);

    assert(tf_key.tweak[0] == three_256_01_tweak[0]);
    assert(tf_key.tweak[1] == three_256_01_tweak[1]); 

    assert(tf_key.stateSize == Threefish256);
    printf(" passed\n");
}

//Test that setting up a 512 bit key works
static void testSetupKey512()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish512, (uint64_t*)three_512_01_key, (uint64_t*)three_512_01_tweak);
    printf("testSetupKey512()");
    assert(tf_key.key[0] == three_512_01_key[0]);
    assert(tf_key.key[1] == three_512_01_key[1]);
    assert(tf_key.key[2] == three_512_01_key[2]);
    assert(tf_key.key[3] == three_512_01_key[3]);
    assert(tf_key.key[4] == three_512_01_key[4]);
    assert(tf_key.key[5] == three_512_01_key[5]);
    assert(tf_key.key[6] == three_512_01_key[6]);
    assert(tf_key.key[7] == three_512_01_key[7]);

    assert(tf_key.tweak[0] == three_512_01_tweak[0]);
    assert(tf_key.tweak[1] == three_512_01_tweak[1]);

    assert(tf_key.stateSize == Threefish512);
    printf(" passed\n");
}

//test that setting up a 1024 bit key works
static void testSetupKey1024()
{
    static ThreefishKey_t tf_key;
    threefishSetKey(&tf_key, Threefish1024, (uint64_t*)three_1024_01_key, (uint64_t*)three_1024_01_tweak);
    printf("testSetupKey1024()");
    assert(tf_key.key[0] == three_1024_01_key[0]);
    assert(tf_key.key[1] == three_1024_01_key[1]);
    assert(tf_key.key[2] == three_1024_01_key[2]);
    assert(tf_key.key[3] == three_1024_01_key[3]);
    assert(tf_key.key[4] == three_1024_01_key[4]);
    assert(tf_key.key[5] == three_1024_01_key[5]);
    assert(tf_key.key[6] == three_1024_01_key[6]);
    assert(tf_key.key[7] == three_1024_01_key[7]);
    assert(tf_key.key[8] == three_1024_01_key[8]);
    assert(tf_key.key[9] == three_1024_01_key[9]);
    assert(tf_key.key[10] == three_1024_01_key[10]);
    assert(tf_key.key[11] == three_1024_01_key[11]);
    assert(tf_key.key[12] == three_1024_01_key[12]);
    assert(tf_key.key[13] == three_1024_01_key[13]);
    assert(tf_key.key[14] == three_1024_01_key[14]);
    assert(tf_key.key[15] == three_1024_01_key[15]);

    assert(tf_key.tweak[0] == three_1024_01_tweak[0]);
    assert(tf_key.tweak[1] == three_1024_01_tweak[1]);

    assert(tf_key.stateSize == Threefish1024);
    printf(" passed\n");
}

//test that 256bits null encrypted matches the test vector
static void testThreefish256NullEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_256[SAFE_SIZE] = { 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish256, (uint64_t*)three_256_00_key, (uint64_t*)three_256_00_tweak);
    threefishEncryptBlockWords(&tf_key, (uint64_t*)three_256_00_plain_text, (uint64_t*)test_block_256); 

    printf("threefish256NullEncrypt()");
    //pdebug("Actual: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)test_block_256);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)three_256_00_expected_result);

    assert(test_block_256[0] == three_256_00_expected_result[0]);
    assert(test_block_256[1] == three_256_00_expected_result[1]);
    assert(test_block_256[2] == three_256_00_expected_result[2]);
    assert(test_block_256[3] == three_256_00_expected_result[3]);
    printf(" passed\n");
}

//test that the 256 non null encrypted with a non null tweak match the test vector 
static void testThreefish256TVEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_256[SAFE_SIZE] = { 0L, 0L, 0L, 0L };
   
    threefishSetKey(&tf_key, Threefish256, (uint64_t*)three_256_01_key, (uint64_t*)three_256_01_tweak);
    threefishEncryptBlockWords(&tf_key, (uint64_t*)three_256_01_plain_text, (uint64_t*)test_block_256);

    printf("threefish256TestVectorEncrypt()");
    /* plaintext feed forward */
    for (uint64_t i = 0; i < SAFE_SIZE; i++) { test_block_256[i] ^= three_256_01_plain_text[i]; }

    //pdebug("Actual: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)test_block_256);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)three_256_01_expected_result);

    assert(test_block_256[0] == three_256_01_expected_result[0]);
    assert(test_block_256[1] == three_256_01_expected_result[1]);
    assert(test_block_256[2] == three_256_01_expected_result[2]);
    assert(test_block_256[3] == three_256_01_expected_result[3]);
    printf(" passed\n");
}

static void testThreefish512NullEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_512[SECURE_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish512, (uint64_t*)three_512_00_key, (uint64_t*)three_512_00_tweak);
    threefishEncryptBlockWords(&tf_key, (uint64_t*)three_512_00_plain_text, (uint64_t*)test_block_512);

    printf("testThreefish512NullEncrypt()");
    assert(test_block_512[0] == three_512_00_expected_result[0]);
    assert(test_block_512[1] == three_512_00_expected_result[1]);
    assert(test_block_512[2] == three_512_00_expected_result[2]);
    assert(test_block_512[3] == three_512_00_expected_result[3]);
    assert(test_block_512[4] == three_512_00_expected_result[4]);
    assert(test_block_512[5] == three_512_00_expected_result[5]);
    assert(test_block_512[6] == three_512_00_expected_result[6]);
    assert(test_block_512[7] == three_512_00_expected_result[7]);
    printf(" passed\n");
}

//test that the 512 non null encrypted with a non null tweak match the test vector 
static void testThreefish512TVEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_512[SECURE_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
   
    threefishSetKey(&tf_key, Threefish512, (uint64_t*)three_512_01_key, (uint64_t*)three_512_01_tweak);
    threefishEncryptBlockWords(&tf_key, (uint64_t*)three_512_01_plain_text, (uint64_t*)test_block_512);
    
    printf("threefish512TestVectorEncrypt()");
    /* plaintext feed forward */
    for (uint64_t i = 0; i < SECURE_SIZE; i++) { test_block_512[i] ^= three_512_01_plain_text[i]; }

    //pdebug("Actual: \n");
    //ShowBuff(Threefish512/8, (uint8_t *)test_block_512);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish512/8, (uint8_t *)three_512_01_expected_result);

    assert(test_block_512[0] == three_512_01_expected_result[0]);
    assert(test_block_512[1] == three_512_01_expected_result[1]);
    assert(test_block_512[2] == three_512_01_expected_result[2]);
    assert(test_block_512[3] == three_512_01_expected_result[3]);
    assert(test_block_512[4] == three_512_01_expected_result[4]);
    assert(test_block_512[5] == three_512_01_expected_result[5]);
    assert(test_block_512[6] == three_512_01_expected_result[6]);
    assert(test_block_512[7] == three_512_01_expected_result[7]);
    printf(" passed\n");
}

//test that 1024 bits null encrypted matches the test vector
static void testThreefish1024NullEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_1024[FP_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish1024, (uint64_t*)three_1024_00_key, (uint64_t*)three_1024_00_tweak);
    threefishEncryptBlockWords(&tf_key, (uint64_t*)three_1024_00_plain_text, (uint64_t*)test_block_1024); 
    printf("threefish1024NullEncrypt()");

    //pdebug("Actual: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)test_block_1024);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)three_1024_01_expected_result);

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
    printf(" passed\n");
}

//test that the 1024 non null encrypted with a non null tweak match the test vector 
static void testThreefish1024TVEncrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_1024[FP_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish1024, (uint64_t*)three_1024_01_key, (uint64_t*)three_1024_01_tweak);
    threefishEncryptBlockWords(&tf_key, (uint64_t*)three_1024_01_plain_text, (uint64_t*)test_block_1024);

    printf("threefish1024TestVectorEncrypt()");
    /* plaintext feed forward */
    for (uint64_t i = 0; i < FP_SIZE; i++) { test_block_1024[i] ^= three_1024_01_plain_text[i]; }

    //pdebug("Actual: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)test_block_1024);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)three_1024_01_expected_result);

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
    printf(" passed\n");
}


static void testThreefish256NullDecrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_256[SECURE_SIZE] = { 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish256, (uint64_t*)three_256_00_key, (uint64_t*)three_256_00_tweak);
    threefishDecryptBlockWords(&tf_key, (uint64_t*)three_256_00_expected_result, (uint64_t*)test_block_256);

    printf("threefish256NullDecrypt()");
    //pdebug("Actual: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)test_block_256);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)three_256_00_plain_text);

    assert(test_block_256[0] == 0);
    assert(test_block_256[1] == 0);
    assert(test_block_256[2] == 0);
    assert(test_block_256[3] == 0);
    printf(" passed\n");
}

static void testThreefish256TVDecrypt()
{
    static ThreefishKey_t tf_key;
    //decryption works in reverse so we want to get the original plain text value from the cipher text
    uint64_t test_block_256[SAFE_SIZE] = { three_256_01_expected_result[0], three_256_01_expected_result[1], three_256_01_expected_result[2], three_256_01_expected_result[3] };

    threefishSetKey(&tf_key, Threefish256, (uint64_t*)three_256_01_key, (uint64_t*)three_256_01_tweak);

    printf("threefish256TVDecrypt()");
    // xor with simulated IV
    for (uint64_t i = 0; i < SAFE_SIZE; i++) { test_block_256[i] ^= three_256_01_plain_text[i]; }
    threefishDecryptBlockWords(&tf_key, test_block_256, test_block_256);

    //pdebug("Actual: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)test_block_256);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish256/8, (uint8_t *)three_256_01_plain_text);

    assert(test_block_256[0] == three_256_01_plain_text[0]);
    assert(test_block_256[1] == three_256_01_plain_text[1]);
    assert(test_block_256[2] == three_256_01_plain_text[2]);
    assert(test_block_256[3] == three_256_01_plain_text[3]);
    printf(" passed\n");
}

static void testThreefish512NullDecrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_512[SECURE_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish512, (uint64_t*)three_512_00_key, (uint64_t*)three_512_00_tweak);
    threefishDecryptBlockWords(&tf_key, (uint64_t*)three_512_00_expected_result, (uint64_t*)test_block_512);

    printf("threefish512NullDecrypt()");
    //pdebug("Actual: \n");
    //ShowBuff(Threefish512/8, (uint8_t *)test_block_512);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish512/8, (uint8_t *)three_512_00_plain_text);

    assert(test_block_512[0] == 0);
    assert(test_block_512[1] == 0);
    assert(test_block_512[2] == 0);
    assert(test_block_512[3] == 0);
    assert(test_block_512[4] == 0);
    assert(test_block_512[5] == 0);
    assert(test_block_512[6] == 0);
    assert(test_block_512[7] == 0);
    printf(" passed\n");
}

static void testThreefish512TVDecrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_512[SECURE_SIZE] = { three_512_01_expected_result[0], three_512_01_expected_result[1], three_512_01_expected_result[2], three_512_01_expected_result[3], three_512_01_expected_result[4], three_512_01_expected_result[5], three_512_01_expected_result[6], three_512_01_expected_result[7] };

    // xor with simulated IV
    for (uint64_t i = 0; i < SECURE_SIZE; i++) { test_block_512[i] ^= three_512_01_plain_text[i]; }

    threefishSetKey(&tf_key, Threefish512, (uint64_t*)three_512_01_key, (uint64_t*)three_512_01_tweak);
    threefishDecryptBlockWords(&tf_key, (uint64_t*)test_block_512, (uint64_t*)test_block_512);
    
    printf("threefish512TVDecrypt()");
    //pdebug("Actual: \n");
    //ShowBuff(Threefish512/8, (uint8_t *)test_block_512);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish512/8, (uint8_t *)three_512_01_plain_text);

    assert(test_block_512[0] == three_512_01_plain_text[0]);
    assert(test_block_512[1] == three_512_01_plain_text[1]);
    assert(test_block_512[2] == three_512_01_plain_text[2]);
    assert(test_block_512[3] == three_512_01_plain_text[3]);
    assert(test_block_512[4] == three_512_01_plain_text[4]);
    assert(test_block_512[5] == three_512_01_plain_text[5]);
    assert(test_block_512[6] == three_512_01_plain_text[6]);
    assert(test_block_512[7] == three_512_01_plain_text[7]);
    printf(" passed\n");
}


static void testThreefish1024NullDecrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_1024[FP_SIZE] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };

    threefishSetKey(&tf_key, Threefish1024, (uint64_t*)three_1024_00_key, (uint64_t*)three_1024_00_tweak);
    threefishDecryptBlockWords(&tf_key, (uint64_t*)three_1024_00_expected_result, (uint64_t*)test_block_1024);

    printf("threefish1024NullDecrypt()");
    //pdebug("Actual: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)test_block_1024);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)three_1024_00_plain_text);

    assert(test_block_1024[0] == 0);
    assert(test_block_1024[1] == 0);
    assert(test_block_1024[2] == 0);
    assert(test_block_1024[3] == 0);
    assert(test_block_1024[4] == 0);
    assert(test_block_1024[5] == 0);
    assert(test_block_1024[6] == 0);
    assert(test_block_1024[7] == 0);
    assert(test_block_1024[8] == 0);
    assert(test_block_1024[9] == 0);
    assert(test_block_1024[10] == 0);
    assert(test_block_1024[11] == 0);
    assert(test_block_1024[12] == 0);
    assert(test_block_1024[13] == 0);
    assert(test_block_1024[14] == 0);
    assert(test_block_1024[15] == 0);
    printf(" passed\n");
}

static void testThreefish1024TVDecrypt()
{
    static ThreefishKey_t tf_key;
    uint64_t test_block_1024[FP_SIZE] = { three_1024_01_expected_result[0], three_1024_01_expected_result[1], three_1024_01_expected_result[2], three_1024_01_expected_result[3], three_1024_01_expected_result[4], three_1024_01_expected_result[5], three_1024_01_expected_result[6], three_1024_01_expected_result[7], three_1024_01_expected_result[8], three_1024_01_expected_result[9], three_1024_01_expected_result[10], three_1024_01_expected_result[11], three_1024_01_expected_result[12], three_1024_01_expected_result[13], three_1024_01_expected_result[14], three_1024_01_expected_result[15] };

    //xor with simulated IV
    for (uint64_t i = 0; i < FP_SIZE; i++) { test_block_1024[i] ^= three_1024_01_plain_text[i]; }

    threefishSetKey(&tf_key, Threefish1024, (uint64_t*)three_1024_01_key, (uint64_t*)three_1024_01_tweak);
    threefishDecryptBlockWords(&tf_key, test_block_1024, test_block_1024);
    
    printf("threefish1024TVDecrypt()");
    //pdebug("Actual: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)test_block_1024);
    //pdebug("\n Expected: \n");
    //ShowBuff(Threefish1024/8, (uint8_t *)three_1024_01_plain_text);

    assert(test_block_1024[0] == three_1024_01_plain_text[0]);
    assert(test_block_1024[1] == three_1024_01_plain_text[1]);
    assert(test_block_1024[2] == three_1024_01_plain_text[2]);
    assert(test_block_1024[3] == three_1024_01_plain_text[3]);
    assert(test_block_1024[4] == three_1024_01_plain_text[4]);
    assert(test_block_1024[5] == three_1024_01_plain_text[5]);
    assert(test_block_1024[6] == three_1024_01_plain_text[6]);
    assert(test_block_1024[7] == three_1024_01_plain_text[7]);
    assert(test_block_1024[8] == three_1024_01_plain_text[8]);
    assert(test_block_1024[9] == three_1024_01_plain_text[9]);
    assert(test_block_1024[10] == three_1024_01_plain_text[10]);
    assert(test_block_1024[11] == three_1024_01_plain_text[11]);
    assert(test_block_1024[12] == three_1024_01_plain_text[12]);
    assert(test_block_1024[13] == three_1024_01_plain_text[13]);
    assert(test_block_1024[14] == three_1024_01_plain_text[14]);
    assert(test_block_1024[15] == three_1024_01_plain_text[15]);
    printf(" passed\n");
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
    testThreefish256NullDecrypt();
    testThreefish256TVDecrypt();
    testThreefish512NullDecrypt();
    testThreefish512TVDecrypt();
    testThreefish1024NullDecrypt();
    testThreefish1024TVDecrypt();
}
