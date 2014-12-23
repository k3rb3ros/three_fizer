#include "include/macThread.h"

void* authenticateMAC(void* parameters)
{
    pdebug("authenticateMAC()\n");
    bool header = true;
    bool maced = false;
    chunk* mac_chunk = NULL;    
    macParams* params = parameters;
 
    while(*(params->running) && *(params->error) == 0)
    {
        //get the next chunk
        if(mac_chunk == NULL && front(params->in))
        {
             pdebug("*** attempting to deque chunk to mac ***\n");
             pthread_mutex_lock(params->in_mutex);
             mac_chunk = front(params->in);
             if(mac_chunk != NULL && mac_chunk->action != MAC) { deque(params->in); } //if the operation succeeded deque the chunk
             pthread_mutex_unlock(params->in_mutex);
        }

        //check for loop termination conditions
        if(mac_chunk != NULL && (mac_chunk->action == MAC || mac_chunk->action == DONE))
        {    break; }

        //check the header
        if(mac_chunk != NULL && header)
        {
             pdebug("*** checking header ***\n");
             const uint64_t block_byte_size = (params->tf_key->stateSize)/8;
             uint64_t* test_header = calloc(2, block_byte_size);

             if(test_header == NULL) //Sanity check 
             {
                  *(params->error) = MEMORY_ALLOCATION_FAIL;
                  return NULL;
             }
             //test the header in a buffer so we can keep the original header encrypted in memory
             memcpy(test_header, mac_chunk->data, 2*block_byte_size);
             decryptHeader(params->tf_key, test_header);
             if(!checkHeader(test_header, params->file_size, params->tf_key->stateSize))
             {
                  pdebug("*** Header check failed ***\n");
                  pdebug("***Ran header check on chunk of size %lu ***\n", mac_chunk->data_size);
                  *(params->error) = HEADER_CHECK_FAIL;
                  if(test_header != NULL) { free(test_header); }
                  return NULL;
             }

             pdebug("Header check succeeded\n");
             header = false; //header check succeaded
             if(test_header != NULL) { free(test_header); }
        }

        if(mac_chunk != NULL && !maced) //mac the chunk
        {
             pdebug("*** Updating mac on chunk of size %lu ***\n", mac_chunk->data_size);
             mac_chunk->action = params->mac_context->out_action; 
             skeinUpdate(params->mac_context->skein_context_ptr, (const uint8_t*)mac_chunk->data, mac_chunk->data_size);
             maced = true;
        }

        //attempt to queue the chunk
        if(mac_chunk != NULL && maced && !queueIsFull(params->out))
        {
             pdebug("*** Queuing chunk of size %lu ***\n", mac_chunk->data_size);
             pthread_mutex_lock(params->out_mutex);
             enque(mac_chunk, params->out); 
             pthread_mutex_unlock(params->out_mutex);
             maced = false;
             mac_chunk = NULL;
        }
   } //end while loop

    pdebug("*** authenticateMAC() done with first loop ***\n");

    //allocates storage for the calculated mac
    uint64_t* calculated_mac = calloc(params->mac_context->digest_byte_size, sizeof(uint8_t));

    if(calculated_mac == NULL) //Sanity check 
    {
        *(params->error) = MEMORY_ALLOCATION_FAIL;
        return NULL;
    }
    
    pdebug("*** Calculating MAC ***\n"); 
    skeinFinal(params->mac_context->skein_context_ptr, (uint8_t*)calculated_mac);
    chunk* stored_mac_chunk = NULL;

    //attempt to get the MAC appended to the end of the file
    while(*(params->running) && *(params->error) == 0)
    {
        //dequeue the mac chunk
        if(stored_mac_chunk == NULL && front(params->in) != NULL)
        {
            pdebug("*** Reading stored MAC ***\n");
            pthread_mutex_lock(params->in_mutex);
            stored_mac_chunk = front(params->in);
            if(stored_mac_chunk != NULL) { deque(params->in); }
            pthread_mutex_unlock(params->in_mutex);
        }

        if(stored_mac_chunk != NULL) { break; } //check for the termination condition
    }

    if(stored_mac_chunk == NULL || stored_mac_chunk->action != MAC)
    {
        pdebug("*** Unable to queue stored mac chunk ***\n");
        *(params->error) = MAC_CHECK_FAIL;
        if(calculated_mac != NULL) { free(calculated_mac); }
        return NULL;
    }

    uint64_t* stored_mac = stored_mac_chunk->data; 

    //compare the calculated and stored mac return an error if they don't match
    for(uint32_t i=0; i<(params->mac_context->digest_byte_size/8); ++i)
    {
        if(calculated_mac[i] != stored_mac[i])
        {
            pdebug("MAC check failed. File has been tampered with or was not encrypted with this program\n");
            *(params->error) = MAC_CHECK_FAIL;
            free(calculated_mac);
            destroyChunk(stored_mac_chunk);
            return NULL;
        } //if control falls through the MAC is valid
    }

    pdebug("MAC check passed\n");
    //queueDone to signal to asyncWrite that we are done
    while(queueIsFull(params->out));
    pthread_mutex_lock(params->out_mutex);
    queueDone(params->out);
    pthread_mutex_unlock(params->out_mutex);

    //free any remaining allocated resources
    destroyChunk(stored_mac_chunk);
    if(calculated_mac != NULL) { free(calculated_mac); }

    return NULL;
}

void* generateMAC(void* parameters)
{
    pdebug("generateMAC()\n");

    bool chunk_maced = false;
    chunk* update_chunk = NULL;
    macParams* params = parameters;
    const uint64_t mac_size = (uint64_t)params->mac_context->digest_byte_size;
    
    //iterate through the queue until the DONE or MAC flag is received and SkeinMAC everything in it
    while(front(params->in) == NULL || front(params->in)->action == GEN_MAC)
    {
        //attempt to pop a chunk off the queue
        if(update_chunk == NULL && front(params->in) != NULL) 
        {
             chunk_maced = false;
             pthread_mutex_lock(params->in_mutex);
             update_chunk = front(params->in); //get the front chunk in the queue
             deque(params->in);
             pthread_mutex_unlock(params->in_mutex);
        }
        
        //generate the MAC from the chunk
        if(update_chunk != NULL && !chunk_maced)
        {
             pdebug("Updating MAC on chunk of size %lu\n", update_chunk->data_size);
             //change the action to the out action
             update_chunk->action = params->mac_context->out_action; 
             //update the MAC with the current chunk
             skeinUpdate(params->mac_context->skein_context_ptr, (const uint8_t*)update_chunk->data, update_chunk->data_size);
             chunk_maced = true;
        }
        
        //attempt to queue the chunk
        if(update_chunk != NULL)
        {
            pdebug("Queuing chunk to write que of size %lu\n", update_chunk->data_size);
            while(queueIsFull(params->out)); //spin until queue is empty
            pthread_mutex_lock(params->out_mutex);
            enque(update_chunk, params->out);
            pthread_mutex_unlock(params->out_mutex);
            //on a successfull queue set mac chunk to NULL so the next chunk will be MACed
            update_chunk = NULL;
        }
        //otherwise spin and wait for the queue to empty
    }

    if(front(params->in) != NULL && front(params->in)->action == DONE)//if we have reached the end of the in queue then get the MAC and que it to the out que 
    {
        uint64_t* mac = calloc(params->mac_context->digest_byte_size, sizeof(uint8_t));
        skeinFinal(params->mac_context->skein_context_ptr, (uint8_t*)mac);
        chunk* mac_chunk = createChunk();
        mac_chunk->action = MAC;
        mac_chunk->data = mac;
        mac_chunk->data_size = mac_size;

        pthread_mutex_lock(params->out_mutex);
        enque(mac_chunk, params->out); //que the MAC chunk 
        pthread_mutex_unlock(params->out_mutex);
        pdebug("$$$$Queuing MAC chunk to write que of size %lu$$$$\n", mac_size);
        pdebug("$$$$Queuing Done from MAC que$$$$\n");

        while(queueIsFull(params->out)); //spin until the queue is empty
        pthread_mutex_lock(params->out_mutex);
        queueDone(params->out); //Queue the done flag
        pthread_mutex_unlock(params->out_mutex);
    }
    else //something went wrong
    {
        *(params->error) = MAC_GENERATION_FAIL;
    }
    
    return NULL;
}

inline void setUpMacParams(macParams* params,
                     bool* mac_status,
                     bool* running, 
                     MacCtx_t* mac_context,
                     pthread_mutex_t* in_mutex,
                     pthread_mutex_t* out_mutex,
                     queue* in,
                     queue* out,
                     ThreefishKey_t* tf_key,
                     uint32_t* error,
                     uint64_t* file_size)
{
    params->mac_status = mac_status;
    params->running = running;
    params->mac_context = mac_context;
    params->in_mutex = in_mutex;
    params->out_mutex = out_mutex;
    params->in = in;
    params->out = out;
    params->tf_key = tf_key;
    params->error = error;
    params->file_size = file_size;
}
