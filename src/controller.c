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

//Put a done flag into the Queue telling all threads using it that there operation on queued data is complete
static inline bool queueDone(const arguments* args, queue* q)
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
                 newchunk->data_size = file_size;
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

static bool decryptFile(const arguments* args, ThreefishKey_t* tf_key)
{

}

/************************************************************************************* 
* This encrypts all queued data passed to 'in' and puts it in the 'out' queue.
* this function assumes a properly formatted header starting with an IV
* is the first thing queued and an empty chunk with the action set to DONE is the
* last thing queued. 
*/
static bool encryptFile(const arguments* args, ThreefishKey_t* tf_key, queue* in, queue* out) //TODO finish me
{
    /************************************************
    * The enrypted file should be written like this *
    * |HEADER|CIPHER_TEXT|MAC|                      *
    * note the MAC operation must include the IV    *
    * and the header                                *
    *************************************************/
    pdebug("encryptFile()\n");

    bool first_chunk = true;

    while(front(in) != NULL && front(in)->action != DONE)
    {
        //get the first chunk queued
        chunk* encrypt_chunk = front(in); 
        uint64_t* chain = NULL;
        deque(in); //pop it off the queue        

        if(first_chunk)
        {
            uint64_t* iv = NULL;
            encryptInPlace(tf_key, iv, encrypt_chunk->data, 1); //IV should be split out of encrypted data            
            first_chunk = false;
        }
        else
        {
            uint64_t num_blocks = 0; //TODO calculate me
            encryptInPlace(tf_key, chain, encrypt_chunk->data, num_blocks); //encrypt the chunk
        }
        //que the encrypted chunk into the out buffer
        enque(encrypt_chunk, out); 
    }
}

int32_t runThreefizer(const arguments* args)
{
    queue* encryptQueue = createQueue(QUE_SIZE);
    queue* macQueue = createQueue(QUE_SIZE);
    queue* writeQueue = createQueue(QUE_SIZE);
    static int32_t status = SUCCESS;
    static ThreefishKey_t tf_key;
    uint64_t* key = handleKey(args); //generate the key
    uint64_t file_size = 0;

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length, args->file_size);

    threefishSetKey(&tf_key, (ThreefishSize_t)args->state_size, key, tf_tweak);

    if(args->encrypt == true)
    {
        //generate header and queue the file
        if(queueHeader(args, encryptQueue) && queueFile(args, encryptQueue))
        {
            if(queueDone(args, encryptQueue)) { encryptFile(args, &tf_key, encryptQueue, macQueue); }
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
            if(checkHeader(&tf_key, iv, header, &file_size))
            {
                if(checkMAC(tf_key, macQueue))
                {
                    decryptFile(args, &tf_key);
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
