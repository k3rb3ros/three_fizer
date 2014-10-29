#include "skeinTests.h"

static void testSkeinInit256()
{
    static struct SkeinCtx skein_state;
    skeinCtxPrepare(&skein_state, Skein256);
    skeinInit(&skein_state, Skein256);

    pdebug("testSkeinInit256()\n");
    assert(skein_state.skeinSize == 256);
    assert(skein_state.XSave[0] == ExpectedState256[0]);
    assert(skein_state.XSave[1] == ExpectedState256[1]);
    assert(skein_state.XSave[2] == ExpectedState256[2]);
    assert(skein_state.XSave[3] == ExpectedState256[3]);
}

static void testSkeinInit512()
{
    static struct SkeinCtx skein_state;
    skeinCtxPrepare(&skein_state, Skein512);
    skeinInit(&skein_state, Skein512);

    pdebug("testSkeinInit512()\n");
    assert(skein_state.skeinSize == 512);
    assert(skein_state.XSave[0] == ExpectedState512[0]);
    assert(skein_state.XSave[1] == ExpectedState512[1]);
    assert(skein_state.XSave[2] == ExpectedState512[2]);
    assert(skein_state.XSave[3] == ExpectedState512[3]);
    assert(skein_state.XSave[4] == ExpectedState512[4]);
    assert(skein_state.XSave[5] == ExpectedState512[5]);
    assert(skein_state.XSave[6] == ExpectedState512[6]);
    assert(skein_state.XSave[7] == ExpectedState512[7]);
}

static void testSkeinInit1024()
{
    static struct SkeinCtx skein_state;
    skeinCtxPrepare(&skein_state, Skein1024);
    skeinInit(&skein_state, Skein1024);
    
    pdebug("testSkeinInit1024()\n");
    assert(skein_state.skeinSize == 1024);
    assert(skein_state.XSave[0] == ExpectedState1024[0]);
    assert(skein_state.XSave[1] == ExpectedState1024[1]);
    assert(skein_state.XSave[2] == ExpectedState1024[2]);
    assert(skein_state.XSave[3] == ExpectedState1024[3]);
    assert(skein_state.XSave[4] == ExpectedState1024[4]);
    assert(skein_state.XSave[5] == ExpectedState1024[5]);
    assert(skein_state.XSave[6] == ExpectedState1024[6]);
    assert(skein_state.XSave[7] == ExpectedState1024[7]);
    assert(skein_state.XSave[8] == ExpectedState1024[8]);
    assert(skein_state.XSave[9] == ExpectedState1024[9]);
    assert(skein_state.XSave[10] == ExpectedState1024[10]);
    assert(skein_state.XSave[11] == ExpectedState1024[11]);
    assert(skein_state.XSave[12] == ExpectedState1024[12]);
    assert(skein_state.XSave[13] == ExpectedState1024[13]);
    assert(skein_state.XSave[14] == ExpectedState1024[14]);
    assert(skein_state.XSave[15] == ExpectedState1024[15]);
}

static void testSkeinNullHash256()
{
    static uint64_t byte_hash[Skein256/64] = { 0L, 0L, 0L, 0L };
    static uint64_t bit_hash[Skein256/64] = { 0L, 0L, 0L, 0L };
    static struct SkeinCtx skein_state;
    
    skeinCtxPrepare(&skein_state, Skein256);
    skeinInit(&skein_state, Skein256);
    skeinUpdate(&skein_state, &byte_hash, 256/8);
    skeinFinal(&skein_state, &byte_hash);
 
    pdebug("testSkeinNullHash256()\n");
    pdebug("Template Null hash:\n");
    ShowBuff(Skein256/8, (uint8_t*)Skein256NullHash);
    pdebug("Byte \n");
    ShowBuff(Skein256/8, (uint8_t*)byte_hash);

    assert(byte_hash[0] == Skein256NullHash[0]);
    assert(byte_hash[1] == Skein256NullHash[1]);
    assert(byte_hash[2] == Skein256NullHash[2]);
    assert(byte_hash[3] == Skein256NullHash[3]);

    pdebug("Bit \n");
    ShowBuff(Skein256/8, (uint8_t*)bit_hash);
    skeinCtxPrepare(&skein_state, Skein256);
    skeinInit(&skein_state, Skein256);
    skeinUpdateBits(&skein_state, &bit_hash, 256);
    skeinFinal(&skein_state, &bit_hash);

    assert(bit_hash[0] == Skein256NullHash[0]);
    assert(bit_hash[1] == Skein256NullHash[1]);
    assert(bit_hash[2] == Skein256NullHash[2]);
    assert(bit_hash[3] == Skein256NullHash[3]);
    
}

static void testSkeinNullHash512()
{
    static uint64_t byte_hash[Skein512/64] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
    static uint64_t bit_hash[Skein512/64] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
    static struct SkeinCtx skein_state;

    skeinCtxPrepare(&skein_state, Skein512);
    skeinInit(&skein_state, Skein512);
    skeinUpdate(&skein_state, &byte_hash, 512/8);
    skeinFinal(&skein_state, &byte_hash);

    pdebug("testSkeinNullHash512()\n");
    pdebug("Template Null hash:\n");
    ShowBuff(Skein512/8, (uint8_t*)Skein512NullHash);
    pdebug("Byte hash:\n");
    ShowBuff(Skein512/8, (uint8_t*)byte_hash);

    assert(byte_hash[0] == Skein512NullHash[0]);
    assert(byte_hash[1] == Skein512NullHash[1]);
    assert(byte_hash[2] == Skein512NullHash[2]);
    assert(byte_hash[3] == Skein512NullHash[3]);
    assert(byte_hash[4] == Skein512NullHash[4]);
    assert(byte_hash[5] == Skein512NullHash[5]);
    assert(byte_hash[6] == Skein512NullHash[6]);
    assert(byte_hash[7] == Skein512NullHash[7]);

    pdebug("Bit hash:\n");
    ShowBuff(Skein512/8, (uint8_t*)bit_hash);
    skeinCtxPrepare(&skein_state, Skein512);
    skeinInit(&skein_state, Skein512);
    skeinUpdateBits(&skein_state, bit_hash, 512);
    skeinFinal(&skein_state, &bit_hash);

    assert(bit_hash[0] == Skein512NullHash[0]);
    assert(bit_hash[1] == Skein512NullHash[1]);
    assert(bit_hash[2] == Skein512NullHash[2]);
    assert(bit_hash[3] == Skein512NullHash[3]);
    assert(byte_hash[4] == Skein512NullHash[4]);
    assert(byte_hash[5] == Skein512NullHash[5]);
    assert(byte_hash[6] == Skein512NullHash[6]);
    assert(byte_hash[7] == Skein512NullHash[7]);
}

static void testSkeinNullHash1024()
{
    static uint64_t byte_hash[Skein1024/64] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
    static uint64_t bit_hash[Skein1024/64] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
    static struct SkeinCtx skein_state;

    skeinCtxPrepare(&skein_state, Skein1024);
    skeinInit(&skein_state, Skein1024);
    skeinUpdate(&skein_state, &byte_hash, 1024/8);
    skeinFinal(&skein_state, &byte_hash);

    pdebug("testSkeinNullHash1024()\n");
    pdebug("Template Null hash:\n");
    ShowBuff(Skein1024/8, (uint8_t*)Skein1024NullHash);
    pdebug("Byte hash:\n");
    ShowBuff(Skein1024/8, (uint8_t*)byte_hash);

    assert(byte_hash[0] == Skein1024NullHash[0]);
    assert(byte_hash[1] == Skein1024NullHash[1]);
    assert(byte_hash[2] == Skein1024NullHash[2]);
    assert(byte_hash[3] == Skein1024NullHash[3]);
    assert(byte_hash[4] == Skein1024NullHash[4]);
    assert(byte_hash[5] == Skein1024NullHash[5]);
    assert(byte_hash[6] == Skein1024NullHash[6]);
    assert(byte_hash[7] == Skein1024NullHash[7]);
    assert(byte_hash[8] == Skein1024NullHash[8]);
    assert(byte_hash[9] == Skein1024NullHash[9]);
    assert(byte_hash[10] == Skein1024NullHash[10]);
    assert(byte_hash[11] == Skein1024NullHash[11]);
    assert(byte_hash[12] == Skein1024NullHash[12]);
    assert(byte_hash[13] == Skein1024NullHash[13]);
    assert(byte_hash[14] == Skein1024NullHash[14]);
    assert(byte_hash[15] == Skein1024NullHash[15]);

    pdebug("Bit hash:\n");
    ShowBuff(Skein1024/8, (uint8_t*)bit_hash);

    skeinCtxPrepare(&skein_state, Skein1024);
    skeinInit(&skein_state, Skein1024);
    skeinUpdateBits(&skein_state, bit_hash, 1024);
    skeinFinal(&skein_state, &bit_hash);

    assert(byte_hash[0] == Skein1024NullHash[0]);
    assert(byte_hash[1] == Skein1024NullHash[1]);
    assert(byte_hash[2] == Skein1024NullHash[2]);
    assert(byte_hash[3] == Skein1024NullHash[3]);
    assert(byte_hash[4] == Skein1024NullHash[4]);
    assert(byte_hash[5] == Skein1024NullHash[5]);
    assert(byte_hash[6] == Skein1024NullHash[6]);
    assert(byte_hash[7] == Skein1024NullHash[7]);
    assert(byte_hash[8] == Skein1024NullHash[8]);
    assert(byte_hash[9] == Skein1024NullHash[9]);
    assert(byte_hash[10] == Skein1024NullHash[10]);
    assert(byte_hash[11] == Skein1024NullHash[11]);
    assert(byte_hash[12] == Skein1024NullHash[12]);
    assert(byte_hash[13] == Skein1024NullHash[13]);
    assert(byte_hash[14] == Skein1024NullHash[14]);
    assert(byte_hash[15] == Skein1024NullHash[15]);
}

static void testSkeinWordHash256()
{
    static uint64_t test_digest[Skein256/64] = { 0L, 0L, 0L, 0L };
    static struct SkeinCtx skein_state;

    skeinCtxPrepare(&skein_state, Skein256);
    skeinInit(&skein_state, Skein256);
    skeinUpdateBits(&skein_state, &TestMsg, 128);
    skeinFinal(&skein_state, test_digest);

    pdebug("testSkeinWordHash256()\n");
    pdebug("Actual\n");
    ShowBuff(Skein256/8, (uint8_t*)test_digest);
    pdebug("Expected\n");
    ShowBuff(Skein256/8, (uint8_t*)ExpectedDigest256);

    assert(test_digest[0] == ExpectedDigest256[0]);
    assert(test_digest[1] == ExpectedDigest256[1]);
    assert(test_digest[2] == ExpectedDigest256[2]);
    assert(test_digest[3] == ExpectedDigest256[3]);
}

static void testSkeinWordHash512()
{
    static uint64_t test_digest[Skein512/64] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
    static struct SkeinCtx skein_state;

    skeinCtxPrepare(&skein_state, Skein512);
    skeinInit(&skein_state, Skein512);
    skeinUpdateBits(&skein_state, &TestMsg, 128);
    skeinFinal(&skein_state, test_digest);

    pdebug("testSkeinWordHash512()\n");
    pdebug("Actual\n");
    ShowBuff(Skein512/8, (uint8_t*)test_digest);
    pdebug("Expected\n");
    ShowBuff(Skein512/8, (uint8_t*)ExpectedDigest512);

    assert(test_digest[0] == ExpectedDigest512[0]);
    assert(test_digest[1] == ExpectedDigest512[1]);
    assert(test_digest[2] == ExpectedDigest512[2]);
    assert(test_digest[3] == ExpectedDigest512[3]);
    assert(test_digest[4] == ExpectedDigest512[4]);
    assert(test_digest[5] == ExpectedDigest512[5]);
    assert(test_digest[6] == ExpectedDigest512[6]);
    assert(test_digest[7] == ExpectedDigest512[7]);
}


static void testSkeinWordHash1024()
{
    static uint64_t test_digest[Skein1024/64] = { 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L, 0L };
    static struct SkeinCtx skein_state;

    skeinCtxPrepare(&skein_state, Skein1024);
    skeinInit(&skein_state, Skein1024);
    skeinUpdateBits(&skein_state, &TestMsg, 128);
    skeinFinal(&skein_state, test_digest);

    pdebug("testSkeinWordHash1024()\n");
    pdebug("Actual\n");
    ShowBuff(Skein1024/8, (uint8_t*)test_digest);
    pdebug("Expected\n");
    ShowBuff(Skein1024/8, (uint8_t*)ExpectedDigest1024);

    assert(test_digest[0] == ExpectedDigest1024[0]);
    assert(test_digest[1] == ExpectedDigest1024[1]);
    assert(test_digest[2] == ExpectedDigest1024[2]);
    assert(test_digest[3] == ExpectedDigest1024[3]);
    assert(test_digest[4] == ExpectedDigest1024[4]);
    assert(test_digest[5] == ExpectedDigest1024[5]);
    assert(test_digest[6] == ExpectedDigest1024[6]);
    assert(test_digest[7] == ExpectedDigest1024[7]);
    assert(test_digest[8] == ExpectedDigest1024[8]);
    assert(test_digest[9] == ExpectedDigest1024[9]);
    assert(test_digest[10] == ExpectedDigest1024[10]);
    assert(test_digest[11] == ExpectedDigest1024[11]);
    assert(test_digest[12] == ExpectedDigest1024[12]);
    assert(test_digest[13] == ExpectedDigest1024[13]);
    assert(test_digest[14] == ExpectedDigest1024[14]);
    assert(test_digest[15] == ExpectedDigest1024[15]);
}

void runSkeinTests()
{
    testSkeinInit256();
    testSkeinInit512();
    testSkeinInit1024();
    testSkeinNullHash256();
    testSkeinNullHash512();
    testSkeinNullHash1024();
    testSkeinWordHash256();
    testSkeinWordHash512();
    testSkeinWordHash1024();
}
