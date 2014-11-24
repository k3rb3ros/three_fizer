#include "include/controller.h"

//A sanity check for decrypting any files smaller then 3 blocks couldn't have been encrypted by this program
static inline bool isAtLeastThreeBlocks(arguments* args)
{
    //FUCK YOUR COUCH!
    return args != NULL && (args->file_size >= ((uint64_t)(args->state_size/8) * 3));
}

/* This takes the argument and hashed the user specified password into a key of block size
* or uses the user entered key directly if they have elected to use this option
* the key is then hashed again and this is ued as the hmac key.
* It assumes the user has correctly allocated block size buffers for the cipher and mac key and passes them into the function
*/
static bool handleKeys(const arguments* args, 
                       ThreefishKey_t* cipher_context, 
                       MacCtx_t* mac_context)
{
    pdebug("handleKeys()\n");
    if(cipher_context == NULL && mac_context == NULL) { return false; } //sanity check

    const uint64_t block_byte_size = (uint64_t)args->state_size/8;
    uint64_t* cipher_key = NULL;
    uint64_t* mac_key = NULL;

    if(args->hash == true)
    {
        //hash the user entered password so the key matches state size
        cipher_key = (uint64_t*)sf_hash(args->password, args->pw_length, args->state_size);
    }
    else
    {
        //use the user entered password as the key directly
        cipher_context = noHashKey(args->password, args->pw_length, args->state_size);
    }

    //generate the mac key from the cipher_key
    mac_key = (uint64_t*)sf_hash(cipher_context, block_byte_size, args->state_size);

    //initialize the key structure for the cipher key
    threefishSetKey(cipher_context, (ThreefishSize_t)args->state_size, cipher_key, cipher_tweak);
    //initialize the mac context and undelying skein structures
    InitMacCtx(mac_context, mac_key, block_byte_size, args->state_size);

    //free allocated resources
    if(cipher_key != NULL) { free(cipher_key); }
    if(mac_key != NULL) { free(mac_key); }

    return true;
}

//Put a done flag into the Queue telling all threads using it that there operation on queued data is complete
static inline bool queueDone(queue* q)
{
    pdebug("queueDone()\n");
    bool success = false;
    chunk* done = createChunk();
    done->action = DONE;

    if(enque(done, q)) { success = true; }

    return success; 
} 

//queue IV and Header pair into cryptoQueue. Must be called before queueFile
static bool queueHeader(const arguments* args, queue* q)
{
    pdebug("queueHeader()\n");
    bool success = false;
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8);
    uint64_t* iv = getRand((uint64_t) args->state_size);

    chunk* header = createChunk();
    header->action = ENCRYPT;
    header->data = genHeader(iv, args->file_size, args->state_size); 
    header->data_size = 2*block_byte_size;
    
    if(header->data != NULL) //check that allocate succeeded
    {
        if(enque(header, q)) { success = true; }
        else 
        { 
            destroyChunk(header);
            perror("Error allocating memory for header cannot continue\n"); 
        }
    }

    if(iv != NULL) { free(iv); }
    return success;
} 

//Queues the file into the que passed in if threefizer is running in decrypt mode the header is queud as a seperate chunk
static bool queueFile(const arguments* args, queue* q) //right now this is blocking until the entire file is queued TODO put me on my own thread so MAC and ENCRYPTION can take place as the file is buffered not just after it
{
    pdebug("queueFile()\n");
    bool header = false;
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8); //get the threefish block size
    const uint64_t header_byte_size = 2*(block_byte_size);
    uint64_t file_size = getSize(args->argz); //get the file size in bytes
    FILE* read = openForBlockRead(args->argz); //open a handle to the file

    if(!args->encrypt) { header = true; pdebug("Decrypt mode queuing header\n"); } //if we are decrypting then we need to look for a header

    while(file_size > 0)
    {
         pdebug("queueFile looping\n");
         if(!queueIsFull(q))
         {
              chunk* new_chunk = createChunk(); //create a new chunk        
              if(args->encrypt) { new_chunk->action = ENCRYPT; } //set the action flag
              else { new_chunk->action = DECRYPT; }            
              if(header) //decrypt mode then read the header into its own check to make checking it simpler
              {
                  if(!isAtLeastThreeBlocks(args)) //sanity check
                  
	          {
                      destroyChunk(new_chunk);
                      fclose(read);
                      return false;
                  }
                  new_chunk->data = readBlock(header_byte_size, read);
                  new_chunk->data_size = header_byte_size;
                  file_size -= header_byte_size;
              }
              else //Normal Operation (not queueing just the header)
              {
                  //read the file into chunks
                  if(file_size < MAX_CHUNK_SIZE)
                  {
                      new_chunk->data = pad(readBlock(file_size, read), file_size, args->state_size);
                      new_chunk->data_size = getPadSize(file_size, args->state_size);
                      file_size -= file_size;
                  }
                  else
                  {
                      //read a chunk of the file
                      new_chunk->data = readBlock(MAX_CHUNK_SIZE, read);
                      new_chunk->data_size = MAX_CHUNK_SIZE;
                      file_size -= MAX_CHUNK_SIZE; //subtract the chunk size from the counter
                  }
              }
              if(new_chunk->data == NULL) //file read failed for whatever reason
              { 
                  destroyChunk(new_chunk);
                  fclose(read);
                  return false; 
              } 
              if(enque(new_chunk, q))
              {
                  if(header) { header = false; }
              }
              else
              {
                  destroyChunk(new_chunk);
                  fclose(read);
                  return false; 
              }
         }
         //otherwise spin (this will create an infinite loop until threading is introduced)
    }

    fclose(read); //close the file handle
    return true;
}

//decrypt the header and check if it is valid
static bool headerIsValid(const ThreefishKey_t* tf_key, 
                          chunk* header, 
                          uint64_t* file_size)
{
    if(decryptHeader(tf_key, header->data))
    {
        if(checkHeader(header->data, file_size, (uint32_t)tf_key->stateSize))
        {
            return true;
        }
    }
    return false;
}

static int decryptBody(const ThreefishKey_t* tf_key, 
                       const uint64_t body_size,
                       const uint64_t* chain,  
                       queue* in,
                       queue* out)
{

    pdebug("decryptBody()\n");
    int status = SUCCESS;
    uint64_t bytes_decrypted = 0;
    uint64_t count = 0;
    const uint64_t block_byte_size = (tf_key->stateSize)/8;
    const uint64_t block_uint64_size = (tf_key->stateSize)/64;
    const uint64_t* even_chain = calloc(block_uint64_size, sizeof(uint64_t));
    const uint64_t* odd_chain = calloc(block_uint64_size, sizeof(uint64_t));
     
    //copy the last cipher text block of the header to the even chain to start things off
    if(memcpy(even_chain, chain, block_byte_size) == NULL) { status = MEMORY_ALLOCATION_FAIL; }

    while(front(in) != NULL && 
          front(in)->action != DONE && 
          bytes_decrypted < body_size && 
          status == SUCCESS)
    {
       bool even = ((count++)%2 == 0) ? true : false;
       chunk* decrypt_chunk = front(in); //get the next encrypted chunk in the queue
       decrypt_chunk->action = WRITE;
       deque(in); //pop it off the queue   
       uint64_t num_blocks = getNumBlocks(decrypt_chunk->data_size, (uint64_t)tf_key->stateSize); //get the size of the chunk

       //decrypt the chunk using even odd alternation to save the the previous chain to be xored into the next chunk
       if(even) 
       {
           if(getChainInBuffer(decrypt_chunk->data, odd_chain, num_blocks, (uint32_t)tf_key->stateSize) != true)
            { status = MEMORY_ALLOCATION_FAIL; }  
            decryptInPlace(tf_key, even_chain, decrypt_chunk->data, num_blocks);
       }
       else
       { 
            if(getChainInBuffer(decrypt_chunk->data, even_chain, num_blocks, (uint32_t)tf_key->stateSize) != true)
            { status = MEMORY_ALLOCATION_FAIL; }
            decryptInPlace(tf_key, odd_chain, decrypt_chunk->data, num_blocks);
       }
       bytes_decrypted += decrypt_chunk->data_size;

       //put the decrypted chunk into the out queue
       if(enque(decrypt_chunk, out) != true) { status = QUEUE_OPERATION_FAIL; }
    }

    if(even_chain != NULL) { free(even_chain); }
    if(odd_chain != NULL) { free(odd_chain); }
    return status;
}

/************************************************************************************ 
* This encrypts all queued data passed to 'in' and puts it in the 'out' queue.
* this function assumes a properly formatted header starting with an IV
* is the first thing queued and an empty chunk with the action set to DONE is the
* last thing queued. 
*************************************************************************************/
static bool encryptChunks(const ThreefishKey_t* tf_key, queue* in, queue* out)//TODO remove header generation and queuing from this function
{
    /********************************************************
    * The encrypted file should be written like this        *
    * |HEADER|CIPHER_TEXT|MAC|                              *
    * note the MAC operation must include the entire header *
    * which in turn includes the IV                         *
    ********************************************************/
    pdebug("encryptChunks()\n");

    bool first_chunk = true;
    bool success = true;
    uint64_t* chain = NULL;

    while(front(in) != NULL && front(in)->action != DONE && success)
    {
        //get the top chunk in the queue
        chunk* encrypt_chunk = front(in); 
        deque(in); //pop it off the queue        

        if(first_chunk)
        {
            encryptHeader(tf_key, encrypt_chunk->data);
            chain = getChainInPlace(encrypt_chunk->data, 2, (uint32_t)tf_key->stateSize); //save the chain of cipher text for use in the next chunk           
            first_chunk = false;
        }
        else
        {
            uint64_t num_blocks = getNumBlocks(encrypt_chunk->data_size, (uint64_t)tf_key->stateSize);
            encryptInPlace(tf_key, chain, encrypt_chunk->data, num_blocks); //encrypt the chunk
            chain = getChainInPlace(encrypt_chunk->data, num_blocks, (uint32_t)tf_key->stateSize);
        }
        //que the encrypted chunk into the out buffer
        encrypt_chunk->action = MAC;
        if(enque(encrypt_chunk, out) != true) { success = false; } 
    }
 
    pdebug("Before return\n");
    return success;
}

//Write everything queued to file
static bool asyncWrite(const arguments* args, 
                       const char* file_name, 
                       uint64_t header_file_size, 
                       queue* q)
{
    pdebug("asyncWrite(args, %s, %lu, q)\n", file_name, header_file_size);
    FILE* write = openForBlockWrite(file_name);
    uint64_t bytes_written = 0;
    uint64_t bytes_to_write = 0;
    if(header_file_size == 0) { header_file_size = ULLONG_MAX; } //if header_file_size is 0 then set it to max effectively disabling header_file_size limits

    while(q != NULL && front(q)->action != DONE  && bytes_written < header_file_size)
    {
        chunk* chunk_to_write = front(q); //get the next chunk in the queue
        if(deque(q) == false) { return false; } //and pop it off and 

        //ensure we aren't writting padding or attempts
        if(bytes_written + chunk_to_write->data_size > header_file_size)
        { bytes_to_write = header_file_size - bytes_written; }
        else { bytes_to_write = chunk_to_write->data_size; }

        writeBlock((uint8_t*)chunk_to_write->data, bytes_to_write, write); //write it to file
        bytes_written += bytes_to_write;
        destroyChunk(chunk_to_write); //free the chunk 
    }

    fclose(write);
    return true;
}

int32_t runThreefizer(const arguments* args)
{
    queue* cryptoQueue = createQueue(QUE_SIZE);
    queue* macQueue = createQueue(QUE_SIZE);
    queue* writeQueue = createQueue(QUE_SIZE);
    static int32_t status = SUCCESS;
    static ThreefishKey_t tf_key; 
    static MacCtx_t mac_context;
    const uint64_t block_byte_size = (args->state_size/8); 
    uint64_t file_size = 0;

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length, args->file_size);

    handleKeys(args, &tf_key, &mac_context); //generate and initialize the keys

    if(args->encrypt == true)
    { //most of this nesting will be done away with when threading is added to each queue
        //generate header and queue the file
        if(queueHeader(args, cryptoQueue) && queueFile(args, cryptoQueue))
        {
            //queue the done flag and start encryption
            if(queueDone(cryptoQueue)) 
            {
                encryptChunks(&tf_key, cryptoQueue, macQueue);
                if(queueDone(macQueue))
                {
                    uint64_t* mac = NULL;
                    mac_context.out_action = WRITE; 
                    if((mac = genMAC(&mac_context, macQueue, writeQueue)) != NULL)
                    {
                        pdebug("mac [%s]\n", mac);
                        chunk* mac_chunk = createChunk();
                        mac_chunk->data = mac;
                        mac_chunk->data_size = mac_context.digest_byte_size;
                        while(enque(mac_chunk, writeQueue) != true); //spin until the mac is queued
                        
                        queueDone(writeQueue);
                        asyncWrite(args, "encrypted.txt", 0, writeQueue); //which simply writes everything in it to file 
                    }

                } 
            }
            else { status = QUEUE_OPERATION_FAIL; }
        }
        else { status = FILE_IO_FAIL; }
    }
    else //decrypt
    { //the amount of nesting here is sketchy but I don't know how better to do it
        if(queueFile(args, cryptoQueue)) //in decrypt mode the first chunk queued will be the header
        {
            chunk* header = front(cryptoQueue);
            const uint64_t block_uint64_size = (tf_key.stateSize/64);
            uint64_t file_size = 0;
            uint64_t* chain = calloc(block_uint64_size, sizeof(uint64_t));

            queueDone(cryptoQueue); 
            deque(cryptoQueue);

            //save the last block of the encrypted header to use as the chain for the body
            if(getChainInBuffer(header->data, chain, 2, (uint32_t)tf_key.stateSize)) { status = CIPHER_OPERATION_FAIL; }
            //headerIsValid decrypts the header if it is valid
            if(headerIsValid(&tf_key, header, &file_size)) 
            {
                status = decryptBody(&tf_key, file_size, chain, cryptoQueue, writeQueue);
                queueDone(writeQueue);
                asyncWrite(args, "decrypted.txt", file_size, writeQueue);                
            }
            else { status = HEADER_CHECK_FAIL; }
 
            if(chain != NULL) { free(chain); }
            destroyChunk(header);
        }
        else { status = FILE_IO_FAIL; }
    }
    //spin up the crypto thread and sick it on the cryptoQueue
    //run hmac on each encrypted chunk when done queue it to the writeQueue
    //spin up the write thread and sick it on the writeQueue
    //free all allocated resources
    destroyQueue(cryptoQueue);
    destroyQueue(macQueue);
    destroyQueue(writeQueue);

    pdebug("Threefizer operation complete\n");

    return status;
}
