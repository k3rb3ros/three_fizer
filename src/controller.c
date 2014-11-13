#include "include/controller.h"

static uint64_t* handleKey(const arguments* args)
{
    uint64_t* key = NULL;
    if(args->hash == true)
    {
       //hash the user entered password so the key matches state size
       key = (uint64_t*)sf_hash(args->password, args->pw_length, args->state_size);
    }
    else
    {
       //use the user entered password as the key directly
       key = noHashKey(args->password, args->pw_length, args->state_size);
    }

    return key;
}

/*TODO finish me
* this function will run hmac on every element in the hmac queue
* then queue the digest 
*/
static bool hmacData(ThreefishKey_t* hmac_key, queue* in, queue* out)
{
    bool success = true;

    //iterate through the queue and
    while(in != NULL && front(in)->action != DONE && success)
    {
        //pop the current message in the queue out of the hmac stack
        chunk* cipher_text_to_write = front(in); 
        deque(in);
        //run the appropriate hmac function
        genMAC(hmac_key);
        if(enque(cipher_text_to_write, out) != true) { success = false; }
    }

    //queue the HMAC here  

    return success;
} 

//Put a done flag into the Queue telling all threads using it that there operation on queued data is complete
static inline bool queueDone(queue* q)
{
    bool success = false;
    chunk* done = createChunk();
    done->action = DONE;

    if(enque(done, q)) { success = true; }

    return success; 
} 

//queue IV and Header pair into readQueue. Must be called before queueFile
static bool queueHeader(const arguments* args, queue* q)
{
    bool success = false;
    const uint64_t block_byte_size = ((uint64_t)args->state_size/8);
    uint64_t* iv = getRand((uint64_t) args->state_size);
    uint64_t* header_info = genHeader(iv, args->file_size, args->state_size);

    chunk* header = createChunk();
    header->action = ENCRYPT;
    header->data = calloc(1, 2*block_byte_size); 
    header->data_size = 2*block_byte_size;
    
    if(header->data != NULL) //check that allocate succeeded
    {
        //test that header was created in ram
        if(memcpy(header->data, iv, block_byte_size)) 
        { 
            success = true;
            enque(header, q); 
        }
        else 
        { 
            destroyChunk(header);
            perror("Error allocating memory for header cannot continue\n"); 
        }
    }

    if(iv != NULL) { free(iv); }
    if(header_info != NULL) { free(header_info); }
    return success;
} 

static bool queueFile(const arguments* args, queue* q) //right now this is blocking until the entire file is queued TODO put me on my own thread so MAC and ENCRYPTION can take place as the file is buffered not just after it
{
    bool status = true; 
    const uint32_t block_size = ((uint32_t)args->state_size/8); //get the threefish block size
    uint64_t file_size = getSize(args->argz); //get the file size in bytes
    FILE* read = openForBlockRead(args->argz); //open a handle to the file

    while(file_size > 0)
    {
         if(!queueIsFull(q))
         {
             chunk* newchunk = calloc(1, sizeof(chunk)); //create a new chunk
             //set the action flag
             if(args->encrypt) { newchunk->action = ENCRYPT; }
             else { newchunk->action = DECRYPT; }
             
             //read the file into chunks
             if(file_size < MAX_CHUNK_SIZE)
             {
                 newchunk->data = pad(readBlock(file_size, read), file_size, args->state_size);
                 newchunk->data_size = getPadSize(file_size, args->state_size);
                 file_size -= file_size;
             }
             else
             {
                 //read a chunk of the file
                 newchunk->data = readBlock(MAX_CHUNK_SIZE, read);
                 newchunk->data_size = MAX_CHUNK_SIZE;
                 file_size -= MAX_CHUNK_SIZE; //subtract the chunk size from the counter
             }
             if(newchunk->data == NULL) { status = false; } //check if the file read failed
             else { enque(newchunk, q); } //If the file read was succesfull queue the chunk
         }
         //otherwise spin
    }

    fclose(read); //close the file handle

    return status;
}

static bool decryptChunks(const arguments* args, const ThreefishKey_t* tf_key)
{

}

/************************************************************************************* 
* This encrypts all queued data passed to 'in' and puts it in the 'out' queue.
* this function assumes a properly formatted header starting with an IV
* is the first thing queued and an empty chunk with the action set to DONE is the
* last thing queued. 
*/
static bool encryptChunks(ThreefishKey_t* tf_key, queue* in, queue* out) //TODO finish me
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
        //get the first chunk queued
        chunk* encrypt_chunk = front(in); 
        deque(in); //pop it off the queue        

        if(first_chunk)
        {
            uint64_t* iv = encrypt_chunk->data; //the first block of the header is the iv so we can use it as such by passing it in as the iv
            uint64_t* header_data = stripIV(encrypt_chunk->data, tf_key->stateSize); 
            encryptInPlace(tf_key, iv, header_data, 1); //encrypt the header
            chain = getChain(encrypt_chunk->data, (uint64_t)tf_key->stateSize, 2); //save the chain of cipher text for use in the next chunk           
            first_chunk = false;
        }
        else
        {
            uint64_t num_blocks = getNumBlocks(encrypt_chunk->data_size, (uint64_t)tf_key->stateSize);
            encryptInPlace(tf_key, chain, encrypt_chunk->data, num_blocks); //encrypt the chunk
            chain = getChain(encrypt_chunk->data, (uint64_t)tf_key->stateSize, num_blocks);
        }
        //que the encrypted chunk into the out buffer
        if(enque(encrypt_chunk, out) != true) { success = false; } 
    }

    return success;
}

static bool asyncWrite(const arguments* args, queue* q)
{
    pdebug("asyncWrite()\n");
    bool success = true;
    FILE* write = openForBlockWrite("test.fnsa");

    pdebug("chunks in queue %lu\n", q->size);

    while(q != NULL && front(q)->action != DONE && success)
    {
        chunk* chunk_to_write = front(q); //get the next chunk in the queue
        if(deque(q) == false) { success = false; } //and pop it off and check for errors
        pdebug("Writing chunk of size %lu\n", chunk_to_write->data_size);
        writeBlock(chunk_to_write->data, chunk_to_write->data_size, write); //write it to file
        destroyChunk(chunk_to_write); //free the chunk 
    }

    terminateFile(write);
    return success;
}

int32_t runThreefizer(const arguments* args)
{
    queue* encryptQueue = createQueue(QUE_SIZE);
    queue* macQueue = createQueue(QUE_SIZE);
    queue* writeQueue = createQueue(QUE_SIZE);
    static int32_t status = SUCCESS;
    static ThreefishKey_t tf_key; 
    static ThreefishKey_t mac_key; //to be cryptographically secure the mac needs its own key
    uint64_t* key = handleKey(args); //generate the key
    uint64_t file_size = 0;

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length, args->file_size);

    threefishSetKey(&tf_key, (ThreefishSize_t)args->state_size, key, tf_tweak);

    if(args->encrypt == true)
    { //most of this nesting will be done away with when threading is added to each queue
        //generate header and queue the file
        if(queueHeader(args, encryptQueue) && queueFile(args, encryptQueue))
        {
            //queue the done flag and start encryption
            if(queueDone(encryptQueue)) 
            { 
                encryptChunks(&tf_key, encryptQueue, macQueue);
                if(queueDone(macQueue))
                { //TODO finish me. This is temporary just to get encryption working
                    hmacData(&mac_key, macQueue, writeQueue);
                    queueDone(writeQueue);
                    //Do the hmac stuff and then move to the write queue
                    asyncWrite(args, writeQueue); //which simply writes everything in it to file 
                } 
            }
            else { status = QUEUE_OPERATION_FAIL; }
        }
        else { status = FILE_IO_FAIL; }
    }
    else //decrypt
    { //the amount of nesting here is sketchy but I don't know how better to do it
        if(queueFile(args, encryptQueue))
        {
            uint64_t* iv = NULL;
            uint64_t* header = NULL;
            if(checkHeader(&tf_key, header, &file_size))
            {
                if(checkMAC(tf_key, macQueue))
                {
                    decryptChunks(args, &tf_key);
                }
                else { status = MAC_CHECK_FAIL; }
            }
            else { status = HEADER_CHECK_FAIL; }
        }
        else { status = FILE_IO_FAIL; }
    }
    //spin up the crypto thread and sick it on the readQueue
    //spin up the write thread and sick it on the writeQueue
    //free all allocated resources
    if(key != NULL)
    {
        free(key);
    }
    destroyQueue(encryptQueue);
    destroyQueue(macQueue);
    destroyQueue(writeQueue);

    return status;
}
