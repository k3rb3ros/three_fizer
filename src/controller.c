#include "include/controller.h"

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
//static bool encryptChunks(ThreefishKey_t* tf_key, queue* in, queue* out)
//{
    /***********************************************************
    * The encrypted file should be written like this           *
    * |HEADER|CIPHER_TEXT|MAC|                                 *
    * note the MAC operation must include the entire header    *
    * which in turn includes the IV                            *
    * The mac size is the same as the block size of the cipher *
    ************************************************************/
/*    pdebug("encryptChunks()\n");

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
        
        if(encrypt_chunk != NULL && !queueIsFull(out)) //attempt to queue the last encrypted chunk
        {
            encrypt_chunk->action = GEN_MAC; //change the next queued action
            //on success clear the chunk ptr so the next operation can happen
            if(enque(encrypt_chunk, out)) { encrypt_chunk = NULL; }
        } 
    }
 
    return true;
}*/

/*
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

    while(in != NULL && front(in)->action != DONE && bytes_written < header_file_size)
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
}*/

int32_t runThreefizer(const arguments* args)
{
    bool mac_status = true;
    bool running = true;
    bool threads_active = false;
    uint32_t error = 0;
    const uint8_t* temp_file_name = hash((uint8_t*)args->argz, 10, args->state_size); 
    const uint64_t block_byte_size = (args->state_size/8);
    pthread_t read_thread;
    pthread_t crypto_thread;
    pthread_t mac_thread;
    pthread_t write_thread;
    pthread_mutex_t crypto_mutex;
    pthread_mutex_t mac_mutex;
    pthread_mutex_t write_mutex;
    queue* crypto_queue = createQueue(QUE_SIZE);
    queue* mac_queue = createQueue(QUE_SIZE);
    queue* write_queue = createQueue(QUE_SIZE);
    cryptParams crypto_params;
    macParams mac_params;
    readParams read_params;
    writeParams write_params;
    MacCtx_t mac_context;
    static int32_t status = SUCCESS;
    static uint64_t file_size = 0;
    static ThreefishKey_t tf_key; 

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length, args->file_size);

    //Init mutexes
    pthread_mutex_init(&crypto_mutex, NULL);
    pthread_mutex_init(&mac_mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);

    handleKeys(args, &tf_key, &mac_context); //generate and initialize the keys
    if(args->encrypt == true && args->file_size > 0)
    {
        setUpCryptoParams(&crypto_params, args, &running, &tf_key, &crypto_mutex, &mac_mutex, crypto_queue, mac_queue, &error);
        setUpMac(&mac_params, &mac_status, &running, &mac_context, &mac_mutex, &write_mutex, mac_queue, write_queue, &error); 
        setUpReadParams(&read_params, args, &running, &crypto_mutex, crypto_queue, &error);
        setUpWriteParams(&write_params, args, &running, &write_mutex, write_queue, temp_file_name, &error, file_size);
        queueHeader(args, crypto_queue);
        threads_active = true;
        pthread_create(&read_thread, NULL, queueFile, &read_params);
        pthread_create(&crypto_thread, NULL, encryptQueue, &crypto_params);
        pthread_create(&mac_thread, NULL, generateMAC, &mac_params);
        pthread_create(&write_thread, NULL, asyncWrite, &write_params);
    }
    else if(args->file_size == 0)
    {
        perror("File to encrypt is empty aborting\n");
        status = FILE_TOO_SMALL;
    }
    else
    {
        setUpReadParams(&read_params, args, &running, &crypto_mutex, mac_queue, &error);
        threads_active = true;
        pthread_create(&read_thread, NULL, queueFile, &read_params);
        pthread_create(&crypto_thread, NULL, decryptQueue, &crypto_params);
        pthread_create(&write_thread, NULL, asyncWrite, &write_params);
    }

    if(args->encrypt == true)
    { //most of this nesting will be done away with when threading is added to each queue
        //generate header and queue the file
        //if(queueHeader(args, crypto_queue))
        //{
            //queueFile(&read_params);
            /*encryptChunks(&tf_key, crypto_queue, mac_queue);
            if(queueDone(mac_queue))
            {
                uint64_t* mac = NULL;
                mac_context.out_action = WRITE; 
                if((mac = genMAC(&mac_context, mac_queue, write_queue)) != NULL)
                {
                    chunk* mac_chunk = createChunk();
                    mac_chunk->data = mac;
                    mac_chunk->data_size = mac_context.digest_byte_size;
                    while(enque(mac_chunk, write_queue) != true); //this should be its own function
                    queueDone(write_queue);
                    if(asyncWrite(temp_file_name, 0, write_queue))
                    { rename((const char*)temp_file_name, "encrypted.txt"); }
                }
                else { status = MAC_GENERATION_FAIL; }
            }
            else { status = QUEUE_OPERATION_FAIL; }*/
        //}
        //else { status = FILE_IO_FAIL; }
    }
    else //decrypt
    {/* 
        //Files smaller then 4 blocks could not have been made by this program
        if(!isAtLeastFourBlocks(args)) 
        {
            destroyQueue(crypto_queue);
            destroyQueue(mac_queue);
            destroyQueue(write_queue);
            return SIZE_CHECK_FAIL; 
        }
        if(true)//queueFile(args, mac_queue)) //in decrypt mode the first chunk queued will be the header
        {
            chunk* header = front(mac_queue);

            if(headerIsValid(&tf_key, header, &file_size)) 
            {
                pdebug("Header is valid\n");
                uint64_t* generated_mac = genMAC(&mac_context, mac_queue, crypto_queue);
                chunk* expected_mac = front(mac_queue);

                if(checkMAC(expected_mac, (uint8_t*)generated_mac, block_byte_size))
                {
                    pdebug("MAC Check Passed\n");
                    header = front(crypto_queue);
                    uint64_t* chain = getChainInPlace(header->data, 2, tf_key.stateSize);
                    deque(crypto_queue); //pop out the header because we no longer need it
                    status = decryptBody(&tf_key, file_size, chain, crypto_queue, write_queue);
                    destroyChunk(header); //free its memory
                    if(asyncWrite(temp_file_name, file_size, write_queue))
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
        else { status = FILE_IO_FAIL; }*/
    }

    //free all allocated resources
    if(threads_active) 
    { 
        pthread_join(read_thread, NULL);
        pthread_join(crypto_thread, NULL);
        pthread_join(mac_thread, NULL);
        pthread_join(write_thread, NULL);
    }
    if(error != 0) { status = error; } //return the error if 1 occured
    pthread_mutex_destroy(&crypto_mutex);
    pthread_mutex_destroy(&mac_mutex);
    pthread_mutex_destroy(&write_mutex);
    destroyQueue(crypto_queue);
    destroyQueue(mac_queue);
    destroyQueue(write_queue);
    if(temp_file_name != NULL) { free((void*) temp_file_name); }

    pdebug("Threefizer operation complete\n");

    return status;
}
