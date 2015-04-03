#include "include/cbcTests.h"
#include "../include/tweak.h"          //threefizer_tweak the same tweak constant used in program operation

static void test256cbcMultiBlockEncryptDecrypt()
{
     printf("test256cbcMultiBlockEncryptDecrypt()");
     static const uint8_t iv[] =
     { 
         1, 2 ,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 
     };
     static const uint8_t bad_iv[] =
     {
         32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
     };
     static const uint16_t state_size = Threefish256;
     static const uint64_t threefish_256_key[] = { 12345678, 910111213141516, 1718192021222324, 2526272829303132 }; 
     static ThreefishKey_t threefish_256_ctx;
     static uint8_t ref_msg[256] = { 0 }; //this is 8 blocks
     static uint8_t ref_ct[256] = { 0 };
     static uint8_t test_msg_1[256] = { 0 }; 
     static uint8_t test_msg_2[256] = { 0 };
     static uint64_t chain_0[4] = { 0 };
     static uint64_t chain_1[4] = { 0 };
     
     threefishSetKey(&threefish_256_ctx, state_size, (uint64_t*)threefish_256_key, threefizer_tweak); //set up threefish
     strcpy((char*)ref_msg, "Privacy is a human right that is necessary for constructive growth and innovation of society. I do not wish to live in a society in which the government can and does spy on the communication of all citizens at all times. Government surveillance is wrong...");
     memcpy(test_msg_1, ref_msg, 256);
     memcpy(test_msg_2, ref_msg, 256);

     //a regular encrypt decrypt cycle
     for(int i=0; i<256; ++i) { assert(test_msg_1[i] == ref_msg[i]); } //test that test_msg and reg_msg are identical
     //cbc encrypt and decrypt the message and confirm we got what we started with
     encryptInPlace(&threefish_256_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 8); 
     memcpy(ref_ct, test_msg_1, 256); //copy the cipher text to a reference text to compare with later chunked encrypt operations
     decryptInPlace(&threefish_256_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 8);
     for(int i=0; i<256; ++i) { assert(test_msg_1[i] == ref_msg[i]); } //test that test_msg and reg_msg are identical
     
     encryptInPlace(&threefish_256_ctx, (uint64_t*)iv, (uint64_t*)test_msg_2, 8); 
     decryptInPlace(&threefish_256_ctx, (uint64_t*)bad_iv, (uint64_t*)test_msg_2, 8);
     //decrypting with the wrong iv causes the next subsequent block of text be corrupt but blocks after that should be unaffected
     for(int i=0; i<256; ++i)
     { 
          if(i<32) { assert(test_msg_2[i] != ref_msg[i]); }
	  if(i>=32) { assert(test_msg_2[i] == ref_msg[i]); }
     }

     //do a 4 part encrypt to see if chain propagation works correctly
     //This simulates how chunks are broken up to due to threading and queue as you go system
     encryptInPlace(&threefish_256_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 2); //encrypt the first 2 blocks with the iv
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 2, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_256_ctx, chain_0, (uint64_t*)(test_msg_1 + 64), 2); 
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 4, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_256_ctx, chain_0, (uint64_t*)(test_msg_1 + 128), 2); 
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 6, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_256_ctx, chain_0, (uint64_t*)(test_msg_1 + 192), 2);
     //test that the cipher text from the multipart encrypt is the same as the ciphertext from the single part encrypt
     for(int i=0; i<256; ++i) { assert(test_msg_1[i] == ref_ct[i]); } 

     //do a 4 part decrypt to see if chain propigation works correctly
     //if it does we should arrive at the origninal plaintext
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 2, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_256_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 2); // decrypt the first 2 blocks using the iv
     getChainInBuffer((uint64_t*)test_msg_1, chain_1, 4, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_256_ctx, (uint64_t*)chain_0, (uint64_t*)(test_msg_1 + 64), 2);
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 6, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_256_ctx, (uint64_t*)chain_1, (uint64_t*)(test_msg_1 + 128), 2);
     decryptInPlace(&threefish_256_ctx, (uint64_t*)chain_0, (uint64_t*)(test_msg_1 + 192), 2);
     //test that the plain text from the multipart decrypt is the same that we started out with
     for(int i=0; i<256; ++i) { assert(test_msg_1[i] == ref_msg[i]); }

     printf(" passed\n");
}

static void test512cbcMultiBlockEncryptDecrypt()
{
     printf("test512cbcMultiBlockEncryptDecrypt()");
     static const uint8_t iv[] =
     {
         1, 2 ,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64
     };
     static const uint8_t bad_iv[] =
     { 
         32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
         1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 
     };
     static const uint16_t state_size = Threefish512;
     static const uint64_t threefish_512_key[] = 
     { 12345678, 910111213141516, 1718192021222324, 2526272829303132, 2526272829303132, 1718192021222324, 910111213141516, 12345678 }; 
     static ThreefishKey_t threefish_512_ctx;
     static uint8_t ref_msg[512] = { 0 }; //this is 8 blocks
     static uint8_t ref_ct[512] = { 0 };
     static uint8_t test_msg_1[512] = { 0 }; 
     static uint8_t test_msg_2[512] = { 0 };
     static uint64_t chain_0[8] = { 0 };
     static uint64_t chain_1[8] = { 0 };
     
     threefishSetKey(&threefish_512_ctx, state_size, (uint64_t*)threefish_512_key, threefizer_tweak); //set up threefish
     
     strcpy((char*)ref_msg, "Until such time that we are governed by a system where all laws are just, and there is no poverty, opression, or undue discrimination between all groups of the governed. Then there is a place for privacy and there is a place for civil disobedience. In order for this privacy to be observed it has to be respected as a civil liberty. The current system in place by the powers to be does not do that. They have chosen a system of all out surveillance that gathers all data available with little to no oversight ...");
     memcpy(test_msg_1, ref_msg, 512);
     memcpy(test_msg_2, ref_msg, 512);

     //a regular encrypt decrypt cycle
     for(int i=0; i<512; ++i) { assert(test_msg_1[i] == ref_msg[i]); } //test that test_msg and reg_msg are identical
     //cbc encrypt and decrypt the message and confirm we got what we started with
     encryptInPlace(&threefish_512_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 8); 
     memcpy(ref_ct, test_msg_1, 512); //copy the cipher text to a reference text to compare with later chunked encrypt operations
     decryptInPlace(&threefish_512_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 8);
     for(int i=0; i<512; ++i) { assert(test_msg_1[i] == ref_msg[i]); } //test that test_msg and reg_msg are identical
     
     encryptInPlace(&threefish_512_ctx, (uint64_t*)iv, (uint64_t*)test_msg_2, 8); 
     decryptInPlace(&threefish_512_ctx, (uint64_t*)bad_iv, (uint64_t*)test_msg_2, 8);
     //decrypting with the wrong iv causes the next subsequent block of text be corrupt but blocks after that should be unaffected
     for(int i=0; i<512; ++i)
     { 
          if(i<64) { assert(test_msg_2[i] != ref_msg[i]); }
	  if(i>=64) { assert(test_msg_2[i] == ref_msg[i]); }
     }

     //do a 4 part encrypt to see if chain propagation works correctly
     //This simulates how chunks are broken up to due to threading and queue as you go system
     encryptInPlace(&threefish_512_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 2); //encrypt the first 2 blocks with the iv
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 2, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_512_ctx, chain_0, (uint64_t*)(test_msg_1 + 128), 2); 
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 4, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_512_ctx, chain_0, (uint64_t*)(test_msg_1 + 256), 2); 
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 6, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_512_ctx, chain_0, (uint64_t*)(test_msg_1 + 384), 2);
     //test that the cipher text from the multipart encrypt is the same as the ciphertext from the single part encrypt
     for(int i=0; i<512; ++i) { assert(test_msg_1[i] == ref_ct[i]); } 

     //do a 4 part decrypt to see if chain propigation works correctly
     //if it does we should arrive at the origninal plaintext
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 2, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_512_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 2); // decrypt the first 2 blocks using the iv
     getChainInBuffer((uint64_t*)test_msg_1, chain_1, 4, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_512_ctx, (uint64_t*)chain_0, (uint64_t*)(test_msg_1 + 128), 2);
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 6, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_512_ctx, (uint64_t*)chain_1, (uint64_t*)(test_msg_1 + 256), 2);
     decryptInPlace(&threefish_512_ctx, (uint64_t*)chain_0, (uint64_t*)(test_msg_1 + 384), 2);
     //test that the plain text from the multipart decrypt is the same that we started out with
     for(int i=0; i<512; ++i) { assert(test_msg_1[i] == ref_msg[i]); }

     printf(" passed\n");
}

static void test1024cbcMultiBlockEncryptDecrypt()
{
     printf("test1024cbcMultiBlockEncryptDecrypt()");
     static const uint8_t iv[] =
     {
         1, 2 ,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64,
         1, 2 ,3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
	 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 54, 56, 57, 58, 59, 60, 61, 62, 63, 64
     };
     static const uint8_t bad_iv[] =
     { 
         32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
         1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 
         32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
         1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 
     };
     static const uint16_t state_size = Threefish1024;
     static const uint64_t threefish_1024_key[] = 
     { 
         12345678, 910111213141516, 1718192021222324, 2526272829303132, 2526272829303132, 1718192021222324, 910111213141516, 12345678,
	 12345678, 910111213141516, 1718192021222324, 2526272829303132, 2526272829303132, 1718192021222324, 910111213141516, 12345678 
     }; 
     static ThreefishKey_t threefish_1024_ctx;
     static uint8_t ref_msg[1024] = { 0 }; //this is 8 blocks
     static uint8_t ref_ct[1024] = { 0 };
     static uint8_t test_msg_1[1024] = { 0 }; 
     static uint8_t test_msg_2[1024] = { 0 };
     static uint64_t chain_0[16] = { 0 };
     static uint64_t chain_1[16] = { 0 };
     
     threefishSetKey(&threefish_1024_ctx, state_size, (uint64_t*)threefish_1024_key, threefizer_tweak); //set up threefish

     strcpy((char*)ref_msg, "Outside, even through the shut window pane, the world looked cold. Down in the street little eddies of wind were whirling dust and torn paper into spirals, and though the sun was shinning and the sky a harsh blue, there seemed to be no color in anything except the posters that were plastered everywhere. The black-mustachio'd face gazed down from every commanding corner. There was one on the house front immediately opposite. BIG BROTHER IS WATCHING YOU, the caption said, while the dark eyes looked deep into Winston's own. Down at street level another poster, torn at one corner, flapped fitfully in the wind, alternately covering and uncovering the single word INGSOC. In the far distance a helicopter skimmed down between the roofs, overhead for an instant like a blue-bottle, and darted away again with a curving flight. It was the Police Patrol, snooping into people's windows. The patrols did not matter, however. Only the Thought Police mattered. Behind Winston's back the voice from the telescreen was still   ...");
     memcpy(test_msg_1, ref_msg, 1024);
     memcpy(test_msg_2, ref_msg, 1024);

     //a regular encrypt decrypt cycle
     for(int i=0; i<1024; ++i) { assert(test_msg_1[i] == ref_msg[i]); } //test that test_msg and reg_msg are identical
     //cbc encrypt and decrypt the message and confirm we got what we started with
     encryptInPlace(&threefish_1024_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 8); 
     memcpy(ref_ct, test_msg_1, 1024); //copy the cipher text to a reference text to compare with later chunked encrypt operations
     decryptInPlace(&threefish_1024_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 8);
     for(int i=0; i<1024; ++i) { assert(test_msg_1[i] == ref_msg[i]); } //test that test_msg and reg_msg are identical
     
     encryptInPlace(&threefish_1024_ctx, (uint64_t*)iv, (uint64_t*)test_msg_2, 8); 
     decryptInPlace(&threefish_1024_ctx, (uint64_t*)bad_iv, (uint64_t*)test_msg_2, 8);
     //decrypting with the wrong iv causes the next subsequent block of text be corrupt but blocks after that should be unaffected
     for(int i=0; i<1024; ++i)
     { 
          if(i<128) { assert(test_msg_2[i] != ref_msg[i]); }
	  if(i>=128) { assert(test_msg_2[i] == ref_msg[i]); }
     }

     //do a 4 part encrypt to see if chain propagation works correctly
     //This simulates how chunks are broken up to due to threading and queue as you go system
     encryptInPlace(&threefish_1024_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 2); //encrypt the first 2 blocks with the iv
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 2, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_1024_ctx, chain_0, (uint64_t*)(test_msg_1 + 256), 2); 
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 4, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_1024_ctx, chain_0, (uint64_t*)(test_msg_1 + 512), 2); 
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 6, state_size); //get the last block of cipher text
     encryptInPlace(&threefish_1024_ctx, chain_0, (uint64_t*)(test_msg_1 + 768), 2);
     //test that the cipher text from the multipart encrypt is the same as the ciphertext from the single part encrypt
     for(int i=0; i<1024; ++i) { assert(test_msg_1[i] == ref_ct[i]); } 

     //do a 4 part decrypt to see if chain propigation works correctly
     //if it does we should arrive at the origninal plaintext
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 2, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_1024_ctx, (uint64_t*)iv, (uint64_t*)test_msg_1, 2); // decrypt the first 2 blocks using the iv
     getChainInBuffer((uint64_t*)test_msg_1, chain_1, 4, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_1024_ctx, (uint64_t*)chain_0, (uint64_t*)(test_msg_1 + 256), 2);
     getChainInBuffer((uint64_t*)test_msg_1, chain_0, 6, state_size); //get the last block of cipher text
     decryptInPlace(&threefish_1024_ctx, (uint64_t*)chain_1, (uint64_t*)(test_msg_1 + 512), 2);
     decryptInPlace(&threefish_1024_ctx, (uint64_t*)chain_0, (uint64_t*)(test_msg_1 + 768), 2);
     //test that the plain text from the multipart decrypt is the same that we started out with

     for(int i=0; i<1024; ++i) { assert(test_msg_1[i] == ref_msg[i]); }

     printf(" passed\n");
}

void runCBCTests()
{
    test256cbcMultiBlockEncryptDecrypt();
    test512cbcMultiBlockEncryptDecrypt();
    test1024cbcMultiBlockEncryptDecrypt();
}