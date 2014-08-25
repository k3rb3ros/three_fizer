#include "include/cbc.h"

void cbcDecryptInPlace(SkeinSize_t state_size, uint8_t* key, uint64_t* cipher_text, uint64_t num_blocks)
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
    }
}

void cbcEncryptInPlace(SkeinSize_t state_size, uint8_t* key, uint64_t* plain_text, uint64_t num_blocks)
{
    if(key != NULL && plain_text != NULL) 
    {
        ThreefishKey_t tf_key;
        uint32_t uints_per_block = state_size/64;
        uint64_t tweak[2] = { 0L, 0L };
        uint64_t* initialization_vector = (uint64_t)getRand(state_size/8); //Fill our IV with Random or Pseudo random numbers
        
        #ifdef DEBUG
        uint8_t* IV = initialization_vector;
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
        pdebug("Cipher text [%s]\n", plain_text);
        #endif

        threefishSetKey(&tf_key, state_size, key, tweak); //set up the 3fish key structure
	
	switch(state_size)
	{
            case 256: cbc256Encrypt(&tf_key, initialization_vector, plain_text, num_blocks);
            break;
            case 512: cbc512Encrypt(&tf_key, initialization_vector, plain_text, num_blocks);
            break;
            case 1024: cbc1024Encrypt(&tf_key, initialization_vector, plain_text, num_blocks);
            break;
            default:
                perror("Invalid state size cannot continue");
                exit(5);
            break;
	}
	
        //threefishEncryptBlockWords(&tf_key, plain_text, plain_text);
        free(initialization_vector);
    }
}

void cbc256Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{
    
}

void cbc512Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{

}

void cbc1024Encrypt(ThreefishKey_t* key, uint64_t* iv, uint64_t* plain_text, uint64_t num_blocks)
{

}
