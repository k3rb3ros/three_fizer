#include "include/cbc.h"

void cbc256Decrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* cipher_text, uint64_t num_blocks)
{
    uint64_t prev_block_odd[SAFE_SLICE] = {0, 0, 0 ,0}; //allocate 1 block of storage to store the previous cipher text 
    uint64_t prev_block_even[SAFE_SLICE] = {0, 0, 0 ,0}; //allocate 1 block of storage to store the previous cipher text 
    for(uint64_t block=0; block<(num_blocks*SAFE_SLICE); block+=SAFE_SLICE) //run each block through the cipher (in decrypt mode)
    {
        bool even = ((block/SAFE_SLICE)%2 == 0) ? true : false;

	if(even)
	{
	    //save the previous cipher text block if it is even 
            prev_block_even[0] = cipher_text[block]; prev_block_even[1] = cipher_text[block+1]; prev_block_even[2] = cipher_text[block+2]; prev_block_even[3] = cipher_text[block+3];
        }
        else
        {
	    //save the previous cipher text block if it is odd
            prev_block_odd[0] = cipher_text[block]; prev_block_odd[1] = cipher_text[block+1]; prev_block_odd[2] = cipher_text[block+2]; prev_block_odd[3] = cipher_text[block+3];
        }
        threefishDecryptBlockWords(key, &cipher_text[block], &cipher_text[block]); //decrypt the block
        if(block == 0)
        {
            cipher_text[0] ^= iv[0]; cipher_text[1] ^= iv[1]; cipher_text[2] ^= iv[2]; cipher_text[3] ^= iv[3];
        }
        else
        {
            if(even)
            {
                cipher_text[block] ^= prev_block_odd[0]; cipher_text[block+1] ^= prev_block_odd[1]; 
                cipher_text[block+2] ^= prev_block_odd[2]; cipher_text[block+3] ^= prev_block_odd[3];
            }
            else
            {
                cipher_text[block] ^= prev_block_even[0]; cipher_text[block+1] ^= prev_block_even[1]; 
                cipher_text[block+2] ^= prev_block_even[2]; cipher_text[block+3] ^= prev_block_even[3];
            }
        }
    }
}

void cbc512Decrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* cipher_text, uint64_t num_blocks)
{
    uint64_t prev_block_odd[SECURE_SLICE] = {0, 0, 0 ,0 ,0 ,0 ,0 ,0}; //allocate 1 block of storage to store the previous cipher text 
    uint64_t prev_block_even[SECURE_SLICE] = {0, 0, 0 ,0 ,0 ,0 ,0 ,0}; //allocate 1 block of storage to store the previous cipher text 
    for(uint64_t block=0; block<(num_blocks*SECURE_SLICE); block+=SECURE_SLICE) //run each block through the cipher (in decrypt mode)
    {
        bool even = ((block/SECURE_SLICE)%2 == 0) ? true : false;

	if(even)
	{
	    //save the previous cipher text block if it is even 
            prev_block_even[0] = cipher_text[block]; prev_block_even[1] = cipher_text[block+1]; prev_block_even[2] = cipher_text[block+2]; prev_block_even[3] = cipher_text[block+3];
            prev_block_even[4] = cipher_text[block+4]; prev_block_even[5] = cipher_text[block+5]; prev_block_even[6] = cipher_text[block+6]; prev_block_even[7] = cipher_text[block+7];
        }
        else
        {
	    //save the previous cipher text block if it is odd
            prev_block_odd[0] = cipher_text[block]; prev_block_odd[1] = cipher_text[block+1]; prev_block_odd[2] = cipher_text[block+2]; prev_block_odd[3] = cipher_text[block+3];
            prev_block_odd[4] = cipher_text[block+4]; prev_block_odd[5] = cipher_text[block+5]; prev_block_odd[6] = cipher_text[block+6]; prev_block_odd[7] = cipher_text[block+7];
        }

        threefishDecryptBlockWords(key, &cipher_text[block], &cipher_text[block]); //Run the block through the cipher
        if(block == 0) //xor the first block with the initialization vector
        {
            cipher_text[0] ^= iv[0]; cipher_text[1] ^= iv[1]; cipher_text[2] ^= iv[2]; cipher_text[3] ^= iv[3];
            cipher_text[4] ^= iv[4]; cipher_text[5] ^= iv[5]; cipher_text[6] ^= iv[6]; cipher_text[7] ^= iv[7];
        }
        else //xor the current block with the previous block of cipher text
        {
            if(even)
            {
                cipher_text[block] ^= prev_block_odd[0]; cipher_text[block+1] ^= prev_block_odd[1]; 
                cipher_text[block+2] ^= prev_block_odd[2]; cipher_text[block+3] ^= prev_block_odd[3];
                cipher_text[block+4] ^= prev_block_odd[4]; cipher_text[block+5] ^= prev_block_odd[5]; 
                cipher_text[block+6] ^= prev_block_odd[6]; cipher_text[block+7] ^= prev_block_odd[7];
            }
            else
            {
                cipher_text[block] ^= prev_block_even[0]; cipher_text[block+1] ^= prev_block_even[1]; 
                cipher_text[block+2] ^= prev_block_even[2]; cipher_text[block+3] ^= prev_block_even[3];
                cipher_text[block+4] ^= prev_block_even[4]; cipher_text[block+5] ^= prev_block_even[5]; 
                cipher_text[block+6] ^= prev_block_even[6]; cipher_text[block+7] ^= prev_block_even[7];
            }
        }
    }
}

void cbc1024Decrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* cipher_text, uint64_t num_blocks)
{
    uint64_t prev_block_odd[FUTURE_PROOF_SLICE] = {0, 0, 0 ,0 ,0 ,0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0}; //allocate 1 block of storage to store the previous cipher text 
    uint64_t prev_block_even[FUTURE_PROOF_SLICE] = {0, 0, 0 ,0 ,0 ,0 ,0 ,0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0}; //allocate 1 block of storage to store the previous cipher text 
    for(uint64_t block=0; block<(num_blocks*FUTURE_PROOF_SLICE); block+=FUTURE_PROOF_SLICE) //run each block through the cipher (in decrypt mode)
    {
        bool even = ((block/FUTURE_PROOF_SLICE)%2 == 0) ? true : false;
        if(even) //Xor the first plain text block with the plain_text
        {
	    //save the previous cipher text block if it is even 
            prev_block_even[0] = cipher_text[block]; prev_block_even[1] = cipher_text[block+1]; prev_block_even[2] = cipher_text[block+2]; prev_block_even[3] = cipher_text[block+3];
            prev_block_even[4] = cipher_text[block+4]; prev_block_even[5] = cipher_text[block+5]; prev_block_even[6] = cipher_text[block+6]; prev_block_even[7] = cipher_text[block+7];
            prev_block_even[8] = cipher_text[block+8]; prev_block_even[9] = cipher_text[block+9]; prev_block_even[10] = cipher_text[block+10]; prev_block_even[11] = cipher_text[block+11];
            prev_block_even[12] = cipher_text[block+12]; prev_block_even[13] = cipher_text[block+13]; prev_block_even[14] = cipher_text[block+14]; prev_block_even[15] = cipher_text[block+15];
        }
        else
        {
	    //save the previous cipher text block if it is even 
            prev_block_odd[0] = cipher_text[block]; prev_block_odd[1] = cipher_text[block+1]; prev_block_odd[2] = cipher_text[block+2]; prev_block_odd[3] = cipher_text[block+3];
            prev_block_odd[4] = cipher_text[block+4]; prev_block_odd[5] = cipher_text[block+5]; prev_block_odd[6] = cipher_text[block+6]; prev_block_odd[7] = cipher_text[block+7];
            prev_block_odd[8] = cipher_text[block+8]; prev_block_odd[9] = cipher_text[block+9]; prev_block_odd[10] = cipher_text[block+10]; prev_block_odd[11] = cipher_text[block+11];
            prev_block_odd[12] = cipher_text[block+12]; prev_block_odd[13] = cipher_text[block+13]; prev_block_odd[14] = cipher_text[block+14]; prev_block_odd[15] = cipher_text[block+15];
        }

        threefishDecryptBlockWords(key, &cipher_text[block], &cipher_text[block]); //Run the block through the cipher
        if(block == 0) //xor the first block with the initialization vector
        {
            cipher_text[0] ^= iv[0]; cipher_text[1] ^= iv[1]; cipher_text[2] ^= iv[2]; cipher_text[3] ^= iv[3];
            cipher_text[4] ^= iv[4]; cipher_text[5] ^= iv[5]; cipher_text[6] ^= iv[6]; cipher_text[7] ^= iv[7];
            cipher_text[8] ^= iv[8]; cipher_text[9] ^= iv[9]; cipher_text[10] ^= iv[10]; cipher_text[11] ^= iv[11];
            cipher_text[12] ^= iv[12]; cipher_text[13] ^= iv[13]; cipher_text[14] ^= iv[14]; cipher_text[15] ^= iv[15];
        }
        else //xor the current block with the previous block of cipher text
        {
            if(even)
            {
                cipher_text[block] ^= prev_block_odd[0]; cipher_text[block+1] ^= prev_block_odd[1]; 
                cipher_text[block+2] ^= prev_block_odd[2]; cipher_text[block+3] ^= prev_block_odd[3];
                cipher_text[block+4] ^= prev_block_odd[4]; cipher_text[block+5] ^= prev_block_odd[5]; 
                cipher_text[block+6] ^= prev_block_odd[6]; cipher_text[block+7] ^= prev_block_odd[7];
                cipher_text[block+8] ^= prev_block_odd[8]; cipher_text[block+9] ^= prev_block_odd[9]; 
                cipher_text[block+10] ^= prev_block_odd[10]; cipher_text[block+11] ^= prev_block_odd[11];
                cipher_text[block+12] ^= prev_block_odd[12]; cipher_text[block+13] ^= prev_block_odd[13]; 
                cipher_text[block+14] ^= prev_block_odd[14]; cipher_text[block+15] ^= prev_block_odd[15];
            }
            else
            {
                cipher_text[block] ^= prev_block_even[0]; cipher_text[block+1] ^= prev_block_even[1]; 
                cipher_text[block+2] ^= prev_block_even[2]; cipher_text[block+3] ^= prev_block_even[3];
                cipher_text[block+4] ^= prev_block_even[4]; cipher_text[block+5] ^= prev_block_even[5]; 
                cipher_text[block+6] ^= prev_block_even[6]; cipher_text[block+7] ^= prev_block_even[7];
                cipher_text[block+8] ^= prev_block_even[8]; cipher_text[block+9] ^= prev_block_even[9]; 
                cipher_text[block+10] ^= prev_block_even[10]; cipher_text[block+11] ^= prev_block_even[11];
                cipher_text[block+12] ^= prev_block_even[12]; cipher_text[block+13] ^= prev_block_even[13]; 
                cipher_text[block+14] ^= prev_block_even[14]; cipher_text[block+15] ^= prev_block_even[15];
            }
        }
    }
}

void cbc256Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    
    for(uint64_t block=0; block<(num_blocks*SAFE_SLICE); block+=SAFE_SLICE) //run each block through the cipher
    { 
       if(block > 0) //feedback the previous into the next block by xoring them together
       {
           uint64_t offset = block-SAFE_SLICE;
           plain_text[block] ^= plain_text[offset]; plain_text[block+1] ^= plain_text[offset+1];
           plain_text[block+2] ^= plain_text[offset+2]; plain_text[block+3] ^= plain_text[offset+3];
       }
       threefishEncryptBlockWords(key, &plain_text[block], &plain_text[block]);
    }
}

void cbc512Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    plain_text[4] ^= iv[4]; plain_text[5] ^= iv[5]; plain_text[6] ^= iv[6]; plain_text[7] ^= iv[7];

    for(uint64_t block=0; block<(num_blocks*SECURE_SLICE); block+=SECURE_SLICE) //run each block through the cipher
    { 
       if(block > 0) //feedback the previous block into the next block by xoring them together
       {
           uint64_t offset = block-SECURE_SLICE;
           plain_text[block] ^= plain_text[offset]; plain_text[block+1] ^= plain_text[offset+1];
           plain_text[block+2] ^= plain_text[offset+2]; plain_text[block+3] ^= plain_text[offset+3];
           plain_text[block+4] ^= plain_text[offset+4]; plain_text[block+5] ^= plain_text[offset+5];
           plain_text[block+6] ^= plain_text[offset+6]; plain_text[block+7] ^= plain_text[offset+7];
       }
       threefishEncryptBlockWords(key, &plain_text[block], &plain_text[block]); //run the current block through the cipher
    }
}

void cbc1024Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    plain_text[4] ^= iv[4]; plain_text[5] ^= iv[5]; plain_text[6] ^= iv[6]; plain_text[7] ^= iv[7];
    plain_text[8] ^= iv[8]; plain_text[9] ^= iv[9]; plain_text[10] ^= iv[10]; plain_text[11] ^= iv[11];
    plain_text[12] ^= iv[12]; plain_text[13] ^= iv[13]; plain_text[14] ^= iv[14]; plain_text[15] ^= iv[15];
    
    for(uint64_t block=0; block<(num_blocks*FUTURE_PROOF_SLICE); block+=FUTURE_PROOF_SLICE) //run each block through the cipher (in decrypt mode)
    { 
       if(block > 0) //feedback the previous into the next block by xoring them together
       {
           uint64_t offset = block-FUTURE_PROOF_SLICE;
           plain_text[block] ^= plain_text[offset]; plain_text[block+1] ^= plain_text[offset+1];
           plain_text[block+2] ^= plain_text[offset+2]; plain_text[block+3] ^= plain_text[offset+3];
           plain_text[block+4] ^= plain_text[offset+4]; plain_text[block+5] ^= plain_text[offset+5];
           plain_text[block+6] ^= plain_text[offset+6]; plain_text[block+7] ^= plain_text[offset+7];
           plain_text[block+7] ^= plain_text[offset+7]; plain_text[block+8] ^= plain_text[offset+8];
           plain_text[block+9] ^= plain_text[offset+9]; plain_text[block+10] ^= plain_text[offset+10];
           plain_text[block+11] ^= plain_text[offset+11]; plain_text[block+12] ^= plain_text[offset+12];
           plain_text[block+12] ^= plain_text[offset+12]; plain_text[block+13] ^= plain_text[offset+13];
           plain_text[block+14] ^= plain_text[offset+14]; plain_text[block+15] ^= plain_text[offset+15];
       }
       threefishEncryptBlockWords(key, &plain_text[block], &plain_text[block]);
    }
}
