#include "include/macThread.h"

void* authenticateMAC(void* parameters)
{
    pdebug("authenticateMAC()\n");    
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
        pthread_mutex_lock(params->out);
        queueDone(params->out); //Queue the done flag
        pthread_mutex_unlock(params->out);
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
                     uint32_t* error)
{
    params->mac_status = mac_status;
    params->running = running;
    params->mac_context = mac_context;
    params->in_mutex = in_mutex;
    params->out_mutex = out_mutex;
    params->in = in;
    params->out = out;
    params->error = error;
}
