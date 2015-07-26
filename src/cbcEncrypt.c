#include "cbcEncrypt.h"

void cbc256Encrypt(ThreefishKey_t* key,
                   const uint64_t* iv,
                   uint64_t* plain_text,
                   const uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1];
    plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];

    //run each block through the cipher chaining on the previous block 
    for(uint64_t block=0; block<(num_blocks*SAFE_SLICE); block+=SAFE_SLICE)
    {
        if(block > 0) //feedback the previous into the next block by xoring them together
        {
            const uint64_t offset = block-SAFE_SLICE;
            plain_text[block] ^= plain_text[offset];
            plain_text[block+1] ^= plain_text[offset+1];
            plain_text[block+2] ^= plain_text[offset+2];
            plain_text[block+3] ^= plain_text[offset+3];
        }

        //run the current block through the cipher
        threefishEncryptBlockWords(key, &plain_text[block], &plain_text[block]);
    }
}

void cbc512Encrypt(ThreefishKey_t* key,
                   const uint64_t* iv,
                   uint64_t* plain_text,
                   const uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1];
    plain_text[2] ^= iv[2]; plain_text[3] ^= iv[3];
    plain_text[4] ^= iv[4]; plain_text[5] ^= iv[5];
    plain_text[6] ^= iv[6]; plain_text[7] ^= iv[7];

    //run each block through the cipher chaining on the previous block
    for(uint64_t block=0; block<(num_blocks*SECURE_SLICE); block+=SECURE_SLICE)
    {
        //feedback the previous block into the next block by xoring them together
        if(block > 0)
        {
            const uint64_t offset = block-SECURE_SLICE;
            plain_text[block] ^= plain_text[offset];
            plain_text[block+1] ^= plain_text[offset+1];
            plain_text[block+2] ^= plain_text[offset+2];
            plain_text[block+3] ^= plain_text[offset+3];
            plain_text[block+4] ^= plain_text[offset+4];
            plain_text[block+5] ^= plain_text[offset+5];
            plain_text[block+6] ^= plain_text[offset+6];
            plain_text[block+7] ^= plain_text[offset+7];
        }
        //run the current block through the cipher
        threefishEncryptBlockWords(key, &plain_text[block], &plain_text[block]);
    }
}

void cbc1024Encrypt(ThreefishKey_t* key,
                    const uint64_t* iv,
                    uint64_t* plain_text,
                    const uint64_t num_blocks)
{
    //xor the initialization vector with the first block of input
    plain_text[0] ^= iv[0]; plain_text[1] ^= iv[1]; plain_text[2] ^= iv[2];
    plain_text[3] ^= iv[3]; plain_text[4] ^= iv[4]; plain_text[5] ^= iv[5];
    plain_text[6] ^= iv[6]; plain_text[7] ^= iv[7]; plain_text[8] ^= iv[8];
    plain_text[9] ^= iv[9]; plain_text[10] ^= iv[10]; plain_text[11] ^= iv[11];
    plain_text[12] ^= iv[12]; plain_text[13] ^= iv[13]; plain_text[14] ^= iv[14];
    plain_text[15] ^= iv[15];

    //run each block through the cipher chaining on the previous block 
    for(uint64_t block=0; block<(num_blocks*FUTURE_PROOF_SLICE); block+=FUTURE_PROOF_SLICE)
    {
        //feedback the previous block into the next by xoring them together
        if(block > 0)
        {
            const uint64_t offset = block-FUTURE_PROOF_SLICE;
            plain_text[block] ^= plain_text[offset];
            plain_text[block+1] ^= plain_text[offset+1];
            plain_text[block+2] ^= plain_text[offset+2];
            plain_text[block+3] ^= plain_text[offset+3];
            plain_text[block+4] ^= plain_text[offset+4];
            plain_text[block+5] ^= plain_text[offset+5];
            plain_text[block+6] ^= plain_text[offset+6];
            plain_text[block+7] ^= plain_text[offset+7];
            plain_text[block+8] ^= plain_text[offset+8];
            plain_text[block+9] ^= plain_text[offset+9];
            plain_text[block+10] ^= plain_text[offset+10];
            plain_text[block+11] ^= plain_text[offset+11];
            plain_text[block+12] ^= plain_text[offset+12];
            plain_text[block+13] ^= plain_text[offset+13];
            plain_text[block+14] ^= plain_text[offset+14];
            plain_text[block+15] ^= plain_text[offset+15];
        }

        //run the current block through the cipher
        threefishEncryptBlockWords(key, &plain_text[block], &plain_text[block]);
    }
}

void encryptInPlace(ThreefishKey_t* key,
                    const uint64_t* chain,
                    uint64_t* plain_text,
                    const uint64_t num_blocks)
{
    pd2("encryptInPlace(key:%lu, chain:%lu, plain_text:%lu, num_blocks:%lu)\n",
        key->key[0],
        chain[0],
        plain_text[0],
        num_blocks);

    switch(key->stateSize) //call the corresponding cbc Encrypt function
    {
        case 256: cbc256Encrypt(key, chain, plain_text, num_blocks);
        break;
        case 512: cbc512Encrypt(key, chain, plain_text, num_blocks);
        break;
        case 1024: cbc1024Encrypt(key, chain, plain_text, num_blocks);
        break;
        default:
        {
            perror("Invalid state size cannot continue\n");
            exit(9);
        }
        break;
    }
}
