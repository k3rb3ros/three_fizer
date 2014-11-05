#include "include/controller.h"

#define SUCCESS 1
#define FILE_IO_FAIL 2

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

static bool queueFile(arguments* args, queue* readQueue)
{
    bool status = true; //TODO use me
    const uint64_t block_size = ((uint64_t)args->state_size/8L); //get the threefish block size
    uint64_t file_size = getSize(args->argz); //get the file size in bytes
    FILE* read = openForBlockRead(args->argz); //open a handle to the file

    pdebug("queueFile()\n");
    pdebug("blocksize:%lu filesize:%lu \n", block_size, file_size);

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
             free(newchunk);
             enque(&newchunk, readQueue); //queue the chunk
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

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length);

    threefishSetKey(&tf_key, (ThreefishSize_t)args->state_size, key, tf_tweak);
    //queue the file
    if(queueFile(args, readQueue) == false)
    {
       status = FILE_IO_FAIL;
    }
 
    //spin up the crypto thread and sick it on the readQueue

    //spin up the async File/IO thread and sick it on the writeQueue

    //free all allocated resources
    if(key != NULL) //TODO add resource freeing to a function
    {
        free(key);
    }
    destroyQueue(readQueue);
    destroyQueue(writeQueue);

    return status;
}
