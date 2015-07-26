#include "include/writeThread.h"

void* asyncWrite(void* parameters)
{
    pdebug("asyncWrite()\n");
    writeParams* params = parameters;
    int64_t write = openForWrite(params->temp_file_name);

    if(write < 0)
    {
        pdebug("Error opening file for write\n");
        *(params->error) = FILE_IO_FAIL;

        return NULL;
    }

    chunk* write_chunk = NULL;
    uint64_t bytes_written = 0;
    uint64_t bytes_to_write = 0;
    uint64_t write_progress = 0;

    while(*(params->running) && *(params->error) == 0)
    {
        if(write_chunk == NULL) //Get a chunk from the queue
        {
            pthread_mutex_lock(params->mutex);
            if(front(params->in) != NULL)
            {
                write_chunk = front(params->in);
                if(write_chunk != NULL) { deque(params->in); }
            }
            pthread_mutex_unlock(params->mutex);
        }

        //check for termination conditions
        if((write_chunk != NULL && write_chunk->action == DONE) || 
            ((params->file_size) != NULL &&
            *(params->valid) && 
            bytes_written >= *(params->file_size))) 
            //size constraints
        {
            pdebug("^^^^ AsyncWrite() terminating loop ^^^^\n");
            destroyChunk(write_chunk);
            break;
        }

        if(write_chunk != NULL && write_chunk->data != NULL)
        {
            //ensure we aren't writing padding or attack attempts
            if(params->file_size == NULL) { bytes_to_write = write_chunk->data_size; }
            else if(bytes_written + write_chunk->data_size > *(params->file_size))
            { bytes_to_write = *(params->file_size) - bytes_written; }
            else { bytes_to_write = write_chunk->data_size; }

            write_progress += write_chunk->data_size;

            if(!writeBytes((uint8_t*)write_chunk->data, bytes_to_write, write))
            {
                pdebug("^^^^ File I/O Error ^^^^\n");
                *(params->error) = FILE_IO_FAIL;
		        close(write);
                break;
            }
            
            ///write it to the file
            pdebug("^^^^ Writing a chunk of size %lu ^^^^\n", bytes_to_write);
            bytes_written += bytes_to_write;
            destroyChunk(write_chunk); //free the chunk
            write_chunk = NULL; 
        }
        
        if(write_progress > 0) //update the progress bar
	    {
	        if(pthread_mutex_trylock(params->progress->progress_mutex) == 0)
	        {
	            params->progress->progress += write_progress;
		        write_progress = 0;
		        pthread_mutex_unlock(params->progress->progress_mutex);
	        }
	    }	
    }
    
    close(write);

    if(*(params->error) == 0)
    {
        if(params->args->rename) //change the output file to what the user wanted
        { rename((char*)params->temp_file_name, (char*)params->args->rename_file); }
        else //rename the temp file to the original file name (replaces file)
        { rename((char*)params->temp_file_name, (char*)params->args->target_file); }

        //signal any other running threads to stop (in case they are stuck)
        *(params->running) = false;
    }
    
    pdebug("^^^^ writeThread Done ^^^^\n");
    //we are done
    return NULL;  
}

void setUpWriteParams(writeParams* params, 
                      const arguments* args, 
                      bool* running,
                      bool* valid,
                      pthread_mutex_t* mutex, 
                      queue* in, 
                      const uint8_t* temp_file_name,
		              progress_t* progress, 
                      int32_t* error, 
                      uint64_t* file_size)
{
    params->args = args;
    params->running = running;
    params->valid = valid;
    params->mutex = mutex;
    params->in = in;
    params->temp_file_name = temp_file_name;
    params->error = error;
    params->file_size = file_size;
    params->progress = progress;
}
