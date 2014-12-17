#include "include/writeThread.h"

void* asyncWrite(void* parameters)
{
    pdebug("asyncWrite()\n");
    chunk* write_chunk = NULL;
    writeParams* params = parameters;
    FILE* write = openForBlockWrite((const char*)params->temp_file_name);
    uint64_t bytes_written = 0;
    uint64_t bytes_to_write = 0;
    if(params->file_size == 0) { params->file_size = ULLONG_MAX; } //if header_file size 0 then set it to max effectively disabling header_file_size limit

    if(write == NULL)
    {
        pdebug("Error opening file for write\n");
        *(params->error) = FILE_IO_FAIL;
        return NULL;
    }

    while(*(params->running) && *(params->error) == 0 && 
          bytes_written < params->file_size && 
          (front(params->in) == NULL || front(params->in)->action != DONE))    
    {
        if(write_chunk == NULL && front(params->in) != NULL) //Get first chunk in the queue
        {
            pthread_mutex_lock(params->mutex);
            write_chunk = front(params->in);
            deque(params->in);     
            pthread_mutex_unlock(params->mutex);
        }

        if(write_chunk != NULL && write_chunk->data != NULL)
        {
            //ensure we aren't writing padding or attack attempts
            if(bytes_written + write_chunk->data_size > params->file_size)
            { bytes_to_write = params->file_size - bytes_written; }
            else { bytes_to_write = write_chunk->data_size; }

            if(!writeBlock((uint8_t*)write_chunk->data, bytes_to_write, write))
            {
                pdebug("File I/O Error\n");
                *(params->error) = FILE_IO_FAIL;
                break;
            }
            
            //write it to the file
            pdebug("^^^^Writing a chunk of size %lu ^^^^\n", bytes_to_write);
            bytes_written += bytes_to_write;
            destroyChunk(write_chunk); //free the chunk
            write_chunk = NULL; 
        }        
    }
    
    fclose(write);
    if(*(params->error) == 0)
    {
        pdebug("^^^^ renaming %s to %s ^^^^\n", params->temp_file_name, params->args->argz);
        rename((const char*)params->temp_file_name, params->args->argz);
        *(params->running) = false; //signal any other running threads to stop (should be redundant)
    }
    
    pdebug("^^^^ writeThread Done ^^^^\n");
    return NULL;  
}

void setUpWriteParams(writeParams* params, 
                      const arguments* args, 
                      bool* running,
                      pthread_mutex_t* mutex, 
                      queue* in, 
                      const uint8_t* temp_file_name, 
                      uint32_t* error, 
                      uint64_t file_size)
{
    params->args = args;
    params->running = running;
    params->mutex = mutex;
    params->in = in;
    params->temp_file_name = temp_file_name;
    params->error = error;
    params->file_size = file_size;
}
