#include "include/hash.h"

uint8_t* hash(const uint8_t* input, 
              const uint64_t digest_length, 
              const SkeinSize_t state_size)
{
    struct SkeinCtx skein_state; //Skein state
    uint8_t* digest = NULL;

    if(input != NULL && validSize(state_size))
    {
        digest = calloc(digest_length+1, sizeof(uint8_t)); //+1 to ensure digest is a null terminated string    
        skeinCtxPrepare(&skein_state, state_size); //Tell skein what size its state is
        skeinInit(&skein_state, digest_length*8); //Init our hash
        skeinUpdate(&skein_state, input, strlen((char*)input)); //Generate our hash
        skeinFinal(&skein_state, (uint8_t*)digest); //Put the results in our buffer and return it
    }

    return digest;
}

uint64_t* hash_key_from_file(const char* fname, const SkeinSize_t state_size) //TODO make me multithreaded
{
   int64_t fd = openForRead(fname);
   if(fd < 0) { return NULL; }
   
   const uint64_t file_size = getFileSize(fname);
   if(file_size == 0) { return NULL; }

   struct SkeinCtx skein_state;
   uint64_t* hash_chunk = NULL;
   uint64_t bytes_hashed = 0;
   uint64_t bytes_to_hash = file_size;

   uint64_t* key = calloc(state_size/64 , sizeof(uint64_t));
   skeinCtxPrepare(&skein_state, state_size); //Set up the context
   skeinInit(&skein_state, state_size); //Init Skein and tell it how big the digest will be

   while(bytes_hashed < file_size) //iterate through the file and run it through Skein
   {
       uint64_t chunk_size = 0;
       if(bytes_to_hash < HASH_BUFFER_SIZE)
       {
	   chunk_size = bytes_to_hash; 
           hash_chunk = (uint64_t*)readBytes(bytes_to_hash, fd);
       }
       else if(bytes_to_hash >= HASH_BUFFER_SIZE);
       {
	   chunk_size = HASH_BUFFER_SIZE; 
           hash_chunk = (uint64_t*)readBytes(HASH_BUFFER_SIZE, fd); 
       }

       if(hash_chunk == NULL) 
       {
           free(key); 
           return NULL;
       }
       skeinUpdate(&skein_state, (uint8_t*)hash_chunk, chunk_size);
       bytes_hashed += bytes_to_hash;
       free(hash_chunk); 
   }
   
   skeinFinal(&skein_state, (uint8_t*)key); //get the digest and return it

   return key;
}

uint8_t* sf_hash(const uint8_t* input, 
                 const uint64_t input_length, 
                 const SkeinSize_t state_size)
{
    struct SkeinCtx skein_state;
    uint8_t* digest = NULL;

    if(input != NULL && validSize(state_size))
    {
        digest = calloc((state_size/8), sizeof(uint8_t)); //allocate the digest buffer    
        skeinCtxPrepare(&skein_state, state_size); //Tell skein what size its state is
        skeinInit(&skein_state, state_size); //Init our hash
        skeinUpdate(&skein_state, input, input_length); //Generate our hash
        skeinFinal(&skein_state, (uint8_t*)digest); //Put the results in our buffer and return it
    }

    return digest;
}
