#include "include/controller.h"

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
        cipher_key = noHashKey(args->password, args->pw_length, args->state_size);
    }

    //generate the mac key from the cipher_key
    mac_key = (uint64_t*)sf_hash((uint8_t*)cipher_key, block_byte_size, args->state_size);

    //initialize the key structure for the cipher key
    threefishSetKey(cipher_context, (ThreefishSize_t)args->state_size, cipher_key, cipher_tweak);
    //initialize the mac context and undelying skein structures
    InitMacCtx(args, mac_context, mac_key);

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
static bool queueHeader(const arguments* args, queue* out)
{
    pdebug("queueHeader()\n");
    bool success = false;
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8);
    uint64_t* iv = (uint64_t*)getRand((uint64_t) args->state_size);

    chunk* header = createChunk();
    header->action = ENCRYPT;
    header->data = genHeader(iv, args->file_size, args->state_size); 
    header->data_size = 2*block_byte_size;
    
    if(header->data != NULL) //check that allocate succeeded
    {
        while(enque(header, out) != true);
        success = true;
    }

    if(iv != NULL) { free(iv); }
    return success;
} 

//Queues the file into out if threefizer is running in decrypt mode the header is queud as a seperate chunk and the 
static bool queueFile(const arguments* args, queue* out) //right now this is blocking until the entire file is queued TODO put me on my own thread so MAC and ENCRYPTION can take place as the file is buffered not just after it
{
    pdebug("queueFile()\n");
    bool header = false;
    chunk* header_chunk = NULL;
    chunk* data_chunk = NULL;
    chunk* mac_chunk = NULL;        
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8); //get the threefish block size
    const uint64_t header_byte_size = 2*(block_byte_size);
    uint64_t orig_file_size = getSize(args->argz); //get the file size in bytes
    FILE* read = openForBlockRead(args->argz); //open a handle to the file

    if(!args->encrypt) { header = true; pdebug("Decrypt mode queuing header\n"); } //if we are decrypting then we need to look for a header

    //Forgive me
    while(orig_file_size > 0)
    {
        if(!queueIsFull(out))
        {
             //only read if we aren't waiting for something else to get queued
             if(header_chunk == NULL && data_chunk == NULL && mac_chunk == NULL)
             {
                 //The begining of all files encrypted with this program is the header.
                 //We know how big the header will be based on the state size of the
                 //block cipher so we put the header in its own chunk to test it.   
                 if(header) 
                 {
                     header_chunk = createChunk();
                     if(header_chunk == NULL) 
                     {
                         perror("Error allocating memory for file read\n");
                         fclose(read);
                         destroyChunk(header_chunk);
                         return false;
                     }
                     header_chunk->action = CHECK_HEADER;
                     header_chunk->data = (uint64_t*)readBlock(header_byte_size, read);
                     header_chunk->data_size = header_byte_size;
                     orig_file_size -= header_byte_size;
                 }
                 else //Normal Operation
                 {
                     //Files encrypted with this program have a MAC of 1 block appended
                     //to the end of the file. In decrypt mode we assume this MAC is 
                     //present and break it into its own chunk so it can be compared
                     // to the MAC generated by the cipher text.
                     if(!args->encrypt && orig_file_size <= MAX_CHUNK_SIZE)
                     {
                          data_chunk = createChunk();
                          mac_chunk = createChunk();
                     
                          if(data_chunk == NULL || mac_chunk == NULL)
                          {
                              perror("Error allocating memory for file read\n");
                              fclose(read);
                              destroyChunk(data_chunk);
                              destroyChunk(mac_chunk);
                              return false;
                          }
                          //Read the last bit of data into a chunk
                          data_chunk->action = GEN_MAC;
                          data_chunk->data = (uint64_t*)readBlock(orig_file_size-block_byte_size, read);
                          data_chunk->data_size = (orig_file_size-block_byte_size);
                          pdebug("###padsize %lu###\n", data_chunk->data_size);

                          orig_file_size -= (orig_file_size-block_byte_size);
                          //Read the MAC into its own chunk
                          mac_chunk->action = MAC;
                          mac_chunk->data = (uint64_t*)readBlock(orig_file_size, read);

                          mac_chunk->data_size = orig_file_size;
                          orig_file_size = 0; //the MAC is always the last thing in the file
                  }
                  else if(orig_file_size <= MAX_CHUNK_SIZE) //the end of the file in encrypt mode
                  {
                      data_chunk = createChunk();
                      if(data_chunk == NULL) 
                      {
                          perror("Error allocating memory for file read\n");
                          fclose(read);
                          destroyChunk(data_chunk);
                          return false;
                      }
                      data_chunk->action = ENCRYPT;
                      data_chunk->data = pad(readBlock(orig_file_size, read),
                                             orig_file_size,
                                             args->state_size);

                      data_chunk->data_size = getPadSize(orig_file_size, args->state_size);
                      orig_file_size -= orig_file_size;
                  }
                  else //read a full chunk of the file
                  {
                      data_chunk = createChunk();
                      if(data_chunk == NULL) 
                      {
                          perror("Error allocating memory for file read\n");
                          fclose(read);
                          destroyChunk(header_chunk);
                          return false;
                      }
                      data_chunk->action = args->encrypt ? ENCRYPT : GEN_MAC;
                      data_chunk->data = (uint64_t*)readBlock(MAX_CHUNK_SIZE, read);
                      data_chunk->data_size = MAX_CHUNK_SIZE;
                      orig_file_size -= MAX_CHUNK_SIZE; //subtract the chunk size from the counter
                  }
               }
            }
              
            //Queue any read chunks in their prefered order
            if(header_chunk != NULL) //attempt to queue the header
            {
                 if(enque(header_chunk, out)) 
                 {
                      pdebug("Queueing header of size %lu\n", header_chunk->data_size);
                      header = false; 
                      header_chunk = NULL;
                 }
            }
            if(data_chunk != NULL) //attempt to enque the data
            {
                 if(enque(data_chunk, out)) 
                 {
                      pdebug("Queueing data of size %lu\n", data_chunk->data_size); 
                      data_chunk = NULL;
                 }
            }
            if(mac_chunk != NULL) //attempt to enque 
            {
                 if(enque(mac_chunk, out)) 
                 {
                      pdebug("Queueing mac of size %lu\n", mac_chunk->data_size); 
                      mac_chunk = NULL; 
                 }
             }
        } //end of queue full check
       //otherwise keep looping until the queue is not full
    } //end of while loop

    fclose(read); //close the file handle
    queueDone(out); //put the DONE flag in the queue
    return true;
}

/*******************************************************************************************
 * decrypt the header and check if it is valid this is done in a buffer and then freed     *
 * because the encrypted header is still neaded to run the mac check.                      *
 * *****************************************************************************************/
static bool headerIsValid(ThreefishKey_t* tf_key, 
                          chunk* header, 
                          uint64_t* file_size)
{
    if(header == NULL) { return false; } 

    bool success = true;
    const uint64_t header_byte_size = header->data_size; 
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
            pdebug("Header check failed either password is incorrect or not a threefizer encrypted file\n");
        }
    }

    if(header_copy != NULL) { free(header_copy); } //free any allocated resorces

    return success;
}

static int decryptBody(ThreefishKey_t* tf_key, 
                       const uint64_t body_size,
                       const uint64_t* chain,  
                       queue* in,
                       queue* out)
{

    pdebug("decryptBody()\n");
    if(tf_key == NULL || chain == NULL || in == NULL || out == NULL) { return CIPHER_OPERATION_FAIL; }

    int status = SUCCESS;
    uint64_t bytes_decrypted = 0;
    uint64_t count = 0;
    const uint64_t block_byte_size = (tf_key->stateSize)/8;
    const uint64_t block_uint64_size = (tf_key->stateSize)/64;
    uint64_t* even_chain = calloc(block_uint64_size, sizeof(uint64_t));
    uint64_t* odd_chain = calloc(block_uint64_size, sizeof(uint64_t));
    
    memcpy(even_chain, chain, block_byte_size);
 
    //copy the last cipher text block of the header to the even chain to start things off
    if(memcpy(even_chain, chain, block_byte_size) == NULL) { status = MEMORY_ALLOCATION_FAIL; }

    while(front(in) != NULL &&
          front(in)->action != DONE && 
          bytes_decrypted < body_size && 
          status == SUCCESS)
    {
       const bool even = ((count++)%2 == 0) ? true : false;
       chunk* decrypt_chunk = front(in); //get the next encrypted chunk in the queue
       decrypt_chunk->action = WRITE;
       deque(in); //pop it off the queue   
       const uint64_t num_blocks = getNumBlocks(decrypt_chunk->data_size, tf_key->stateSize); //get the size of the chunk

       //decrypt the chunk using even odd alternation to save the last block of the previous chunk to be xored with the first block of the next chunk
       if(even) 
       {
           if(getChainInBuffer(decrypt_chunk->data, odd_chain, num_blocks, tf_key->stateSize) != true)
            { status = MEMORY_ALLOCATION_FAIL; }  
            decryptInPlace(tf_key, even_chain, decrypt_chunk->data, num_blocks);
       }
       else
       { 
            if(getChainInBuffer(decrypt_chunk->data, even_chain, num_blocks, tf_key->stateSize) != true)
            { status = MEMORY_ALLOCATION_FAIL; }
            decryptInPlace(tf_key, odd_chain, decrypt_chunk->data, num_blocks);
       }
       bytes_decrypted += decrypt_chunk->data_size;

       //put the decrypted chunk into the out queue
       if(enque(decrypt_chunk, out) != true) { status = QUEUE_OPERATION_FAIL; }
    }

    queueDone(out);

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
static bool encryptChunks(ThreefishKey_t* tf_key, queue* in, queue* out)
{
    /***********************************************************
    * The encrypted file should be written like this           *
    * |HEADER|CIPHER_TEXT|MAC|                                 *
    * note the MAC operation must include the entire header    *
    * which in turn includes the IV                            *
    * The mac size is the same as the block size of the cipher *
    ************************************************************/
    pdebug("encryptChunks()\n");

    bool first_chunk = true;
    chunk* encrypt_chunk = NULL;
    uint64_t* chain = NULL;

    while(front(in) != NULL && front(in)->action != DONE)
    {
        if(encrypt_chunk == NULL)
        {
            //get the top chunk in the queue
            encrypt_chunk = front(in); 
            deque(in); //pop it off the queue        

            if(first_chunk) //encrypt header
            {
                if(!encryptHeader(tf_key, encrypt_chunk->data)) 
                { 
                    destroyChunk(encrypt_chunk);
                    return false;
                }
                chain = getChainInPlace(encrypt_chunk->data, 2, (uint32_t)tf_key->stateSize); //save the chain of cipher text for use in the next chunk           
                first_chunk = false;
            }
            else
            {
                uint64_t num_blocks = getNumBlocks(encrypt_chunk->data_size, (uint64_t)tf_key->stateSize);
                encryptInPlace(tf_key, chain, encrypt_chunk->data, num_blocks); //encrypt the chunk
                chain = getChainInPlace(encrypt_chunk->data, num_blocks, (uint32_t)tf_key->stateSize);
            }
        }
        //spin until the last chunk has been enqued
        
        if(encrypt_chunk != NULL && !queueIsFull(out) ) //attempt to queue the last encrypted chunk
        {
            encrypt_chunk->action = GEN_MAC; //change the next queued action
            //on success clear the chunk ptr so the next operation can happen
            if(enque(encrypt_chunk, out)) { encrypt_chunk = NULL; }
        } 
    }
 
    return true;
}

//Write everything queued to file
static bool asyncWrite(const uint8_t* file_name, 
                       uint64_t header_file_size, 
                       queue* in)
{
    pdebug("asyncWrite(args, %s, %lu, q)\n", file_name, header_file_size);
    FILE* write = openForBlockWrite((const char*)file_name);
    uint64_t bytes_written = 0;
    uint64_t bytes_to_write = 0;
    if(header_file_size == 0) { header_file_size = ULLONG_MAX; } //if header_file_size is 0 then set it to max effectively disabling header_file_size limits

    while(in != NULL && front(in)->action != DONE  && bytes_written < header_file_size)
    {
        chunk* chunk_to_write = front(in); //get the next chunk in the queue
        if(deque(in) == false) { return false; } // pop it off and 

        //ensure we aren't writing padding or attempts
        if(bytes_written + chunk_to_write->data_size > header_file_size)
        { bytes_to_write = header_file_size - bytes_written; }
        else { bytes_to_write = chunk_to_write->data_size; }

        //write it to the file
        pdebug("Writing a chunk of size of %lu\n", bytes_to_write);
        writeBlock((uint8_t*)chunk_to_write->data, bytes_to_write, write);
        bytes_written += bytes_to_write;
        destroyChunk(chunk_to_write); //free the chunk 
    }

    fclose(write);
    return true;
}

int32_t runThreefizer(const arguments* args)
{
    const uint8_t* temp_file_name = hash((uint8_t*)args->argz, 10, args->state_size); 
    const uint64_t block_byte_size = (args->state_size/8); 
    //const uint64_t block_uint64_size = (args->state_size/64);
    queue* cryptoQueue = createQueue(QUE_SIZE);
    queue* macQueue = createQueue(QUE_SIZE);
    queue* writeQueue = createQueue(QUE_SIZE);
    static int32_t status = SUCCESS;
    static uint64_t file_size = 0;
    static MacCtx_t mac_context;
    static ThreefishKey_t tf_key; 

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length, args->file_size);

    handleKeys(args, &tf_key, &mac_context); //generate and initialize the keys

    if(args->encrypt == true)
    { //most of this nesting will be done away with when threading is added to each queue
        //generate header and queue the file
        if(queueHeader(args, cryptoQueue) && queueFile(args, cryptoQueue))
        {
            encryptChunks(&tf_key, cryptoQueue, macQueue);
            if(queueDone(macQueue))
            {
                uint64_t* mac = NULL;
                mac_context.out_action = WRITE; 
                if((mac = genMAC(&mac_context, macQueue, writeQueue)) != NULL)
                {
                    chunk* mac_chunk = createChunk();
                    mac_chunk->data = mac;
                    mac_chunk->data_size = mac_context.digest_byte_size;
                    while(enque(mac_chunk, writeQueue) != true); //this should be its own function
                    queueDone(writeQueue);
                    if(asyncWrite(temp_file_name, 0, writeQueue))
                    { rename((const char*)temp_file_name, "encrypted.txt"); }
                }
                else { status = MAC_GENERATION_FAIL; }
            }
            else { status = QUEUE_OPERATION_FAIL; }
        }
        else { status = FILE_IO_FAIL; }
    }
    else //decrypt
    { 
        //Files smaller then 4 blocks could not have been made by this program
        if(!isAtLeastFourBlocks(args)) 
        {
            destroyQueue(cryptoQueue);
            destroyQueue(macQueue);
            destroyQueue(writeQueue);
            return SIZE_CHECK_FAIL; 
        }
        if(queueFile(args, macQueue)) //in decrypt mode the first chunk queued will be the header
        {
            chunk* header = front(macQueue);

            if(headerIsValid(&tf_key, header, &file_size)) 
            {
                pdebug("Header is valid\n");
                uint64_t* generated_mac = genMAC(&mac_context, macQueue, cryptoQueue);
                chunk* expected_mac = front(macQueue);

                if(checkMAC(expected_mac, generated_mac, block_byte_size))
                {
                    pdebug("MAC Check Passed\n");
                    header = front(cryptoQueue);
                    uint64_t* chain = getChainInPlace(header->data, 2, tf_key.stateSize);
                    deque(cryptoQueue); //pop out the header because we no longer need it
                    status = decryptBody(&tf_key, file_size, chain, cryptoQueue, writeQueue);
                    destroyChunk(header); //free its memory
                    if(asyncWrite(temp_file_name, file_size, writeQueue))
                    { rename((const char*)temp_file_name, "decrypted.txt"); }
                }   
                else 
                {
                    pdebug("MAC Check Failed\n"); 
                    status = MAC_CHECK_FAIL; 
                }
                
                if(generated_mac != NULL) { free(generated_mac); }                
            }
            else { status = HEADER_CHECK_FAIL; }
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
    if(temp_file_name != NULL) { free((void*) temp_file_name); }

    pdebug("Threefizer operation complete\n");

    return status;
}
