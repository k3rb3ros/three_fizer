#include "include/controller.h"

static uint64_t* handleKey(arguments* args)
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

static bool queueFile(arguments* args, queue* readQueue) //right now this is blocking until the entire file is queued TODO put me on my own thread so MAC and ENCRYPTION can take place as the file is buffered not just after it
{
    bool status = true; //TODO use me
    const uint64_t block_size = ((uint64_t)args->state_size/8L); //get the threefish block size
    uint64_t file_size = getSize(args->argz); //get the file size in bytes
    FILE* read = openForBlockRead(args->argz); //open a handle to the file

    while(file_size > 0)
    {
         if(!queueIsFull(readQueue))
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
             enque(newchunk, readQueue); //queue the chunk
         }
         //otherwise spin
    }

    fclose(read); //close the file handle

    return status;
}

int32_t runThreefizer(arguments* args)
{
    queue* readQueue = createQueue(QUE_SIZE);
    queue* writeQueue = createQueue(QUE_SIZE);
    static int32_t status = SUCCESS;
    static ThreefishKey_t tf_key;
    uint64_t* key = handleKey(args); //generate the key
    uint64_t file_size = 0;

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length);

    threefishSetKey(&tf_key, (ThreefishSize_t)args->state_size, key, tf_tweak);
    //queue the file
    if(queueFile(args, readQueue) == false)
    {
        status = FILE_IO_FAIL;
    }

    if(args->encrypt == true)
    {  
       file_size = getSize(args->argz);
       uint64_t* header = genHeader(&tf_key, file_size, (uint64_t*)getRand(args->state_size), args->state_size);
   
       /************************************************
       * The enrypted file should be written like this *
       * |IV|HEADER|CIPHER_TEXT|MAC|
       *************************************************/
       //encrypt
    }
    else
    {
        uint64_t* iv = NULL;
        uint64_t* header = NULL;
        if(checkHeader(&tf_key, iv, header, &file_size))
        {
            //decrypt
        }
        else
        {
            status = HEADER_CHECK_FAIL;
        }
    }
    //spin up the crypto thread and sick it on the readQueue

    //spin up the async File/IO thread and sick it on the writeQueue

    //free all allocated resources
    if(key != NULL)
    {
        free(key);
    }
    destroyQueue(readQueue);
    destroyQueue(writeQueue);

    return status;
}
