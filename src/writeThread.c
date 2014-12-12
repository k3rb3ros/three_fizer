#include "include/writeThread.h"

void* asyncWrite(void* parameters)
{
    pdebug("asyncWrite()\n");
    writeParams* params = parameters;
    FILE* write = openForBlockWrite((const char*)params->temp_file_name);
    uint64_t bytes_written = 0;
    uint64_t bytes_to_write = 0;
    if(params->file_size == 0) { params->file_size = ULLONG_MAX; } //if header_file size 0 then set it to max effectively disabling header_file_size limit

    while(*(params->running) && *(params->error) == 0 && 
          bytes_written < params->file_size && front(params->in))    
    {
        chunk* chunk_to_write = front(params->in); //get the next chunk in the queue
        if(deque(params->in) == false) 
        {
            perror("Unable to deque chunk\n");
            *(params->error) = QUEUE_OPERATION_FAIL; 
            break; 
        } // pop it off and 

        //ensure we aren't writing padding or attempts
        if(bytes_written + chunk_to_write->data_size > params->file_size)
        { bytes_to_write = params->file_size - bytes_written; }
        else { bytes_to_write = chunk_to_write->data_size; }

        //write it to the file
        pdebug("Writing a chunk of size %lu\n", bytes_to_write);
        if(!writeBlock((uint8_t*)chunk_to_write->data, bytes_to_write, write))
        {
            *(params->error) = FILE_IO_FAIL;
            break;
        }
        bytes_written += bytes_to_write;
        destroyChunk(chunk_to_write); //free the chunk 
    }
    
    fclose(write);
    if(*(params->error) == 0)
    {
        rename((const char*)params->temp_file_name, params->args->argz);
    }
    return NULL;  
}

void setUpWriteParams(writeParams* params, 
                      const arguments* args, 
                      bool* running, 
                      queue* in, 
                      const uint8_t* temp_file_name, 
                      uint32_t* error, 
                      uint64_t file_size)
{
    params->args = args;
    params->running = running;
    params->in = in;
    params->temp_file_name = temp_file_name;
    params->error = error;
    params->file_size = file_size;
}
