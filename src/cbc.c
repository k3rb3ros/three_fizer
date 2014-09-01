#include "include/cbc.h"

void cbcDecryptInPlace(SkeinSize_t state_size, uint64_t* iv, uint8_t* key, uint64_t* cipher_text, uint64_t num_blocks)
{
    if(key != NULL) //this should be moved outside cbc
    {
        #ifdef DEBUG
        pdebug("Key: [");
        for(uint16_t i=0; i<state_size/8; ++i)
        {
            pdebug("%x ", key[i]);
        }
        pdebug("]\n");
        #endif

        ThreefishKey_t tf_key;
        uint64_t tweak[2] = { 0L, 0L };
        threefishSetKey(&tf_key, (ThreefishSize_t)state_size, (uint64_t*)key, tweak); //set up the 3fish key structure        

	switch(state_size) //call the corresponding decrypt function
	{
            case 256: cbc256Decrypt(&tf_key, iv, cipher_text, num_blocks-1);
            break;
            case 512: cbc512Decrypt(&tf_key, iv, cipher_text, num_blocks-1);
            break;
            case 1024: cbc1024Decrypt(&tf_key, iv, cipher_text, num_blocks-1);
            break;
            default:
                perror("Invalid state size cannot continue");
                exit(5);
            break;
	}
    }
}

void cbcEncryptInPlace(SkeinSize_t state_size, uint64_t* iv, uint8_t* key, uint64_t* plain_text, uint64_t num_blocks)
{
    if(key != NULL && plain_text != NULL) 
    {
        #ifdef DEBUG
        uint8_t* IV = (uint8_t*) iv;
        pdebug("Key: [");
        for(uint16_t i=0; i<state_size/8; ++i)
        {
            pdebug("%x ", key[i]);
        }
        pdebug("]\n");
        pdebug("\n\nIV: [");
        for(uint16_t k=0; k<state_size/8; ++k)
        {
            printf("%x ", IV[k]);
        }
        pdebug("]\n");
        #endif

        ThreefishKey_t tf_key;
        uint64_t tweak[2] = { 0L, 0L };
        threefishSetKey(&tf_key, (ThreefishSize_t)state_size, (uint64_t*)key, tweak); //set up the 3fish key structure
	
	switch(state_size) //call the corresponding encrypt function
	{
            case 256: cbc256Encrypt(&tf_key, iv, plain_text, num_blocks);
            break;
            case 512: cbc512Encrypt(&tf_key, iv, plain_text, num_blocks);
            break;
            case 1024: cbc1024Encrypt(&tf_key, iv, plain_text, num_blocks);
            break;
            default:
                perror("Invalid state size cannot continue");
                exit(5);
            break;
	}
    }
}

void cbc256Decrypt() 
{
    
}

void cbc512Decrypt()
{

}

void cbc1024Decrypt()
{

}

void cbc256Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    
    for(uint64_t block=0; block<num_blocks; block+=SAFE_SLICE) //run each block through the cipher
    { 
       if(block > 0) //feedback the previous into the next block by xoring them together
       {
           plain_text[block] ^= plain_text[block-SAFE_SLICE]; plain_text[block+1] ^= plain_text[block-SAFE_SLICE+1];
           plain_text[block+2] ^= plain_text[block-SAFE_SLICE+2]; plain_text[block+3] ^= plain_text[block-SAFE_SLICE+3];
       }
       threefishEncryptBlockWords(key, plain_text, plain_text);
    }
}

void cbc512Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    plain_text[4] ^= iv[4]; plain_text[5] ^= iv[5]; plain_text[6] ^= iv[6]; plain_text[7] ^= iv[7];

    for(uint64_t block=0; block<num_blocks; block+=SECURE_SLICE) //run each block through the cipher
    { 
       if(block > 0) //feedback the previous into the next block by xoring them together
       {
           plain_text[block] ^= plain_text[block-SECURE_SLICE]; plain_text[block+1] ^= plain_text[block-SECURE_SLICE+1];
           plain_text[block+2] ^= plain_text[block-SECURE_SLICE+2]; plain_text[block+3] ^= plain_text[block-SECURE_SLICE+3];
           plain_text[block+4] ^= plain_text[block-SECURE_SLICE+4]; plain_text[block+5] ^= plain_text[block-SECURE_SLICE+5];
           plain_text[block+6] ^= plain_text[block-SECURE_SLICE+6]; plain_text[block+7] ^= plain_text[block-SECURE_SLICE+7];
       }
       threefishEncryptBlockWords(key, plain_text, plain_text);
    }
}

void cbc1024Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    plain_text[4] ^= iv[4]; plain_text[5] ^= iv[5]; plain_text[6] ^= iv[6]; plain_text[7] ^= iv[7];
    plain_text[8] ^= iv[8]; plain_text[9] ^= iv[9]; plain_text[10] ^= iv[10]; plain_text[11] ^= iv[11];
    plain_text[12] ^= iv[12]; plain_text[13] ^= iv[13]; plain_text[14] ^= iv[14]; plain_text[15] ^= iv[15];
    
    for(uint64_t block=0; block<num_blocks; block+=FUTURE_PROOF_SLICE) //run each block through the cipher
    { 
       if(block > 0) //feedback the previous into the next block by xoring them together
       {
           plain_text[block] ^= plain_text[block-FUTURE_PROOF_SLICE]; plain_text[block+1] ^= plain_text[block-FUTURE_PROOF_SLICE+1];
           plain_text[block+2] ^= plain_text[block-FUTURE_PROOF_SLICE+2]; plain_text[block+3] ^= plain_text[block-FUTURE_PROOF_SLICE+3];
           plain_text[block+4] ^= plain_text[block-FUTURE_PROOF_SLICE+4]; plain_text[block+5] ^= plain_text[block-FUTURE_PROOF_SLICE+5];
           plain_text[block+6] ^= plain_text[block-FUTURE_PROOF_SLICE+6]; plain_text[block+7] ^= plain_text[block-FUTURE_PROOF_SLICE+7];
           plain_text[block+7] ^= plain_text[block-FUTURE_PROOF_SLICE+7]; plain_text[block+8] ^= plain_text[block-FUTURE_PROOF_SLICE+8];
           plain_text[block+9] ^= plain_text[block-FUTURE_PROOF_SLICE+9]; plain_text[block+10] ^= plain_text[block-FUTURE_PROOF_SLICE+10];
           plain_text[block+11] ^= plain_text[block-FUTURE_PROOF_SLICE+11]; plain_text[block+12] ^= plain_text[block-FUTURE_PROOF_SLICE+12];
           plain_text[block+12] ^= plain_text[block-FUTURE_PROOF_SLICE+12]; plain_text[block+13] ^= plain_text[block-FUTURE_PROOF_SLICE+13];
           plain_text[block+14] ^= plain_text[block-FUTURE_PROOF_SLICE+14]; plain_text[block+15] ^= plain_text[block-FUTURE_PROOF_SLICE+15];
       }
       threefishEncryptBlockWords(key, plain_text, plain_text);
    }
}
