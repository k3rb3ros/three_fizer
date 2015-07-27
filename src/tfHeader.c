#include "include/tfHeader.h"

/* Checks if the header is valid which is a quick and easy way to tell if the
 * encryption key and block size are correct, with out having to try and decrypt the
 * entire file
*/
bool checkHeader(const uint64_t* header,
                 uint64_t* file_size,
                 const SkeinSize_t state_size)
{
    pdebug("checkHeader()\n");
    const uint32_t iv_offset = ((uint32_t)state_size/64);
    
    if(header != NULL && file_size != NULL && validSize(state_size))
    {
        pdebug("Header{ ");
        pdebug("%lu, ", header[iv_offset+0]);
        pdebug("%lu, ", header[iv_offset+1]);
        pdebug("%lu, ", header[iv_offset+2]);
        pdebug("%lu ", header[iv_offset+3]);
        pdebug("}\n");

        if(header[iv_offset+0] == MAGIC_NUMBER && 
           header[iv_offset+2] == state_size)//if the header check passes
        {
            *file_size = header[iv_offset+1]; //store the unpadded file size
            pdebug("Header check passed, header is valid\n");
            return true;
        }
    }
    return false;
}

/**********************************************************************
* The Header is twice the block size of the cipher                    *
* The first block being the random initialization vector              *
* The second block containg a magic number, the unencrypted file size,*
* the cipher state size and the verison of key generation             *
* magic number and NULL padding if the state size > 256 bits          *
*                  HEADER STRUCTURE                                   *
* |#######################IV########################|                 *
* |MAGIC_NUMBER|DATA_SIZE|STATE_SIZE|VERSION|PADDING|                 *
***********************************************************************/
static uint64_t* genHeader(const uint64_t* iv,
                           const uint64_t data_size,
                           const uint32_t state_size,
                           const uint16_t version)
{
    pdebug("genHeader()\n");
    const uint32_t block_byte_size = (state_size/8);
    const uint32_t block_uint64_size = (state_size/64);
    uint64_t* header = NULL;

    if(iv != NULL && 
       validSize(state_size) &&
       data_size > 1 &&
       (version > 0 && version <= 2) //there are only two valid versions 1 and 2
       )
    {
        //allocate memory for the header
        header = calloc(2*block_uint64_size, sizeof(uint64_t));

        memcpy(header, iv, block_byte_size);
        header[block_uint64_size+0] = MAGIC_NUMBER; 
        header[block_uint64_size+1] = data_size; 
        header[block_uint64_size+2] = state_size;
        if(version == 1)
        { header[block_uint64_size+3] = V1; }
        else if(version == 2)
        { header[block_uint64_size+3] = V2; }

        pdebug("Header{ ");
        pdebug("%lu, ", header[block_uint64_size+0]);
        pdebug("%lu, ", header[block_uint64_size+1]);
        pdebug("%lu, ", header[block_uint64_size+2]);
        pdebug("%lu ", header[block_uint64_size+3]);
        pdebug("}\n");
    }

    return header;
}

/********************************************************************************
* decrypt the header and check if it is valid this is done in a buffer and then
* freed because the encrypted header is still neaded to run the mac check.
********************************************************************************/
bool headerIsValid(ThreefishKey_t* tf_key,
                   chunk* header,
                   uint64_t* file_size)
{
    if(header == NULL) { return false; }

    bool success = true;
    const uint64_t header_byte_size = header->data_size;

    //we make a copy of the header so we can pass just the header
    uint64_t* header_copy = calloc(header_byte_size, sizeof(uint8_t));

    if(header_copy == NULL)
    {
        perror("Error unable to allocate memory for header check\n");
        return false;
    }

    memcpy(header_copy, header->data, header_byte_size);

    if(decryptHeader(tf_key, header_copy))
    {
        if(checkHeader(header_copy, file_size, tf_key->stateSize))
        {
            header->action = GEN_MAC;
            success = true;
        }
        else
        {
            perror("Header check failed either password is incorrect or not a threefizer encrypted file\n");
        }
    }

    return success;
}

//generate a header and put it into the crypto queue
bool queueHeader(arguments* args, queue* out)
{
    pdebug("queueHeader()\n");
    
    if(args->iv == NULL) { return false; } //iv doesn't exist we can't continue

    bool success = false;
    uint16_t header_ver = (args->legacy_hash==true) ? 1 : 2; 
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8);

    chunk* header = createChunk();
    header->action = ENCRYPT;
    header->data = genHeader(args->iv, args->file_size, args->state_size, header_ver);
    header->data_size = 2*block_byte_size;

    if(header->data != NULL) //check that allocate succeeded
    {
        while(enque(header, out) != true); //spin until the header has been queued
        success = true;
    }

    if(args->iv != NULL) //free the iv
    { 
        free(args->iv);
        args->iv = NULL;
    } 

    return success;
}

inline bool genIV(arguments* args)
{
    //generate the iv from /dev/urandom
    uint64_t* iv = (uint64_t*)getRand((uint64_t)args->state_size);

    if(iv != NULL)
    {
        args->iv = iv;
        return true;
    }

    return false;
}

bool getIV(arguments* args)
{
    int64_t read = openForRead(args->target_file);

    if(read > 0)
    {
       uint64_t* iv = (uint64_t*)readBytes((uint64_t)(args->state_size/8), read);
       if(iv != NULL)
       { 
           args->iv = iv;
           return true; 
       }
    }

    return false;
}

/* Knowing the internal structure of the header it is possible to return a pointer
 * directly to the data of the header ignoring the iv*/
inline uint64_t* stripIV(const uint64_t* header, const uint64_t state_size)
{
    return (uint64_t*)header + (state_size/64);
}
