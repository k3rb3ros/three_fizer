#include "include/readThread.h"

void* queueFileForDecrypt(void* parameters)
{
   pdebug("queueFileForDecrypt()\n");
   const readParams* params = parameters;

   if(!isGreaterThanThreeBlocks(params->args))
   {
       *(params->error) = FILE_TOO_SMALL;
       return NULL;
   }

   bool header = true;
   bool mac = false;
   chunk* data_chunk = NULL;
   const uint64_t block_byte_size = ((uint64_t)params->args->state_size/8); //get threefish block size
   const uint64_t header_byte_size = 2*(block_byte_size);
   const uint64_t orig_file_size = getFileSize(params->args->argz); //get the file size in bytes

   uint64_t bytes_read = 0;

   const int64_t read = openForRead(params->args->argz); //open a handle to the file
   if(read < 0)
   {
       *(params->error) = FILE_IO_FAIL;
       return NULL;
   }

   while(*(params->running) && *(params->error) == 0 && bytes_read < orig_file_size)
   {
       //attempt to read a new chunk from the file
       if(data_chunk == NULL)
       {
           uint64_t chunk_size = 0;
           if(header) { chunk_size = header_byte_size; } //2block header prepended to file
           else if(!header && (orig_file_size - (bytes_read + block_byte_size)) >= MAX_CHUNK_SIZE) { chunk_size = MAX_CHUNK_SIZE; } //A full chunk of data
           else if(!header && 
	           (orig_file_size - (bytes_read + block_byte_size)) < MAX_CHUNK_SIZE &&
		   (orig_file_size - (bytes_read + block_byte_size)) > 0)
           { chunk_size = (orig_file_size - (bytes_read + block_byte_size)); }
           else //MAC appended to the end of file
           {
               chunk_size = block_byte_size;
	       mac = true;
           }

           uint64_t* data = (uint64_t*)pad(readBytes(chunk_size, read), chunk_size, params->args->state_size);
           pdebug("### Reading chunk of size %lu ### \n", chunk_size);
           if(data != NULL)
           {
               bytes_read += chunk_size;
	       data_chunk = createChunk();
	       if(header)
	       { 
	           data_chunk->action = CHECK_HEADER;
		   header = false; 
	       }
	       else if(mac) { data_chunk->action = MAC; }
	       else { data_chunk->action = GEN_MAC; }
	       data_chunk->data = data;
	       data_chunk->data_size = chunk_size;   
           }
       }

       //attepmt to queue the chunk
       if(data_chunk!= NULL)
       {
	   pthread_mutex_lock(params->mutex);
	   if(!queueIsFull(params->out) && enque(data_chunk, params->out))
           {
               pdebug("### Queuing data of size %lu ### \n", data_chunk->data_size);
	       data_chunk = NULL;
	   }
	   pthread_mutex_unlock(params->mutex);
       }
       pd2("QueueFileFerDecrypt() Tick\n");
   } //end of thread loop

   //queue done
   while(*(params->running) && *(params->error) == 0 && queueIsFull(params->out)) { ; } //spin unit queue is free
   pthread_mutex_lock(params->mutex);
   if(!queueDone(params->out))
   {
       pdebug("Error queing done\n");
       if(read > 0) { close(read); }
       *(params->error) = QUEUE_OPERATION_FAIL;
       return NULL;
   }
   pthread_mutex_unlock(params->mutex);
   //free allocated resorces
   if(read > 0) { close(read); }

   return NULL;
}

//Queing a file for Encrypt is easy all we have to do is read it into chunks and que them until 
//we reach the end of the file.
void* queueFileForEncrypt(void* parameters)
{
    pdebug("queueFileForEncrypt()\n");

    const readParams* params = parameters;
    const uint64_t orig_file_size = getFileSize(params->args->argz); //get the file size in bytes
    if(orig_file_size == 0) //check that we are actually encrypting something
    {
        *(params->error) = FILE_TOO_SMALL;
	return NULL;
    }

    chunk* data_chunk = NULL;
    
    int64_t read = (openForRead(params->args->argz)); //attempt to open a handle to the file
    if(read < 0)
    {
        *(params->error) = FILE_IO_FAIL;
	return NULL;
    }

    uint64_t bytes_read = 0;

    //Read the File into chunks and queue them while the end hasn't been reached and there is no error
    while(*(params->running) && *(params->error) == 0 && bytes_read < orig_file_size)
    {
	//attepmt to read a new chunk from the file
        if(data_chunk == NULL)
	{
	    //get the largest possible size for the next chunk
	    const uint64_t chunk_size = ((orig_file_size - bytes_read) >= MAX_CHUNK_SIZE) ? MAX_CHUNK_SIZE : (orig_file_size - bytes_read); 
            uint64_t* data = (uint64_t*)pad(readBytes(chunk_size, read), chunk_size, params->args->state_size);
	    bytes_read += chunk_size;
	    pdebug("### Reading chunk of size %lu ### \n", chunk_size);
	    if(data != NULL)
            {
                data_chunk = createChunk();
		data_chunk->action = ENCRYPT;
		data_chunk->data = data;
	        data_chunk->data_size = getPadSize(chunk_size, params->args->state_size);   
	    }
        }

	//attempt to queue the chunk
	if(data_chunk != NULL)
	{
	    pthread_mutex_lock(params->mutex);
            if(!queueIsFull(params->out) && enque(data_chunk, params->out))
	    {
		pdebug("### Queing data of size %lu ### \n", data_chunk->data_size);
                data_chunk = NULL;
	    }
	    pthread_mutex_unlock(params->mutex);
	} //end queue operation
       pd2("QueueFileFerDecrypt() Tick\n");
    } //end thread loop

    //queue done
    while(*(params->running) && *(params->error) == 0 && queueIsFull(params->out)) { ; } //spin unit queue is free
    pthread_mutex_lock(params->mutex);
    if(!queueDone(params->out)) 
    {   //abort if this fails as it will cause a deadlock
        pdebug("Error queuing done\n");
	if(read > 0) { close(read); }
	*(params->error) = QUEUE_OPERATION_FAIL;
	return NULL;
    }
    pthread_mutex_unlock(params->mutex);

    if(read > 0) { close(read); }

    return NULL;
}

inline void setUpReadParams(readParams* read_params, 
                     const arguments* args, 
                     bool* running,
                     pthread_mutex_t* mutex, 
                     queue* out,
                     int32_t* error)
{
    read_params->args = args;
    read_params->running = running;
    read_params->mutex = mutex;
    read_params->out = out;
    read_params->error = error;
}
