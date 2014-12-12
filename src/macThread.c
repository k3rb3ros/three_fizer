#include "include/macThread.h"

void* authenticateMAC(void* parameters)
{
    pdebug("authenticateMAC()\n");    
    return NULL;
}

void* generateMAC(void* parameters)
{
    pdebug("generateMAC()\n");

    chunk* update_chunk = NULL;
    macParams* params = parameters;
    const uint64_t mac_size = (uint64_t)params->mac_context->digest_byte_size;
    
    //iterate through the queue until the DONE or MAC flag is received and SkeinMAC everything in it
    while(front(params->in)->action != DONE && front(params->in)->action != MAC)
    {
        if(update_chunk == NULL)
        {
             update_chunk = front(params->in); //get the front chunk in the queue
             deque(params->in); //pop it off the queue
             
             if(update_chunk->action != GEN_MAC)
             {
                 perror("Bad data in mac queue aborting operation\n");
                 destroyChunk(update_chunk);
                 *(params->error) = MAC_GENERATION_FAIL;
                 break;
             }

             //change the action to the out action
             update_chunk->action = params->mac_context->out_action; 
             //update the MAC with the current chunk
             skeinUpdate(params->mac_context->skein_context_ptr, (const uint8_t*)update_chunk->data, update_chunk->data_size);

        }
        
        //attempt to queue the chunk
        if(update_chunk != NULL && enque(update_chunk, params->out) == true)
        {
            pdebug("Queuing chunk to write que of size %lu\n", update_chunk->data_size);
            //on a successfull queue set mac chunk to NULL so the next chunk will be MACed
            queueDone(params->out);
            update_chunk = NULL;
        }
        //otherwise spin and wait for the queue to empty
    }

    if(params->in != NULL && front(params->in)->action == DONE) //TODO investigate if this is correct
    {
        uint64_t* mac = calloc(params->mac_context->digest_byte_size, sizeof(uint8_t));
        skeinFinal(params->mac_context->skein_context_ptr, (uint8_t*)mac);
        chunk* mac_chunk = createChunk();
        mac_chunk->action = MAC;
        mac_chunk->data = mac;
        mac_chunk->data_size = mac_size;
        while(queueIsFull(params->out)); //spin until the write queue is not full
        enque(mac_chunk, params->out); 
    }
    else //something went wrong
    {
        *(params->error) = MAC_GENERATION_FAIL;
    }
    
    return NULL;
}

inline void setUpMac(macParams* params, 
                     bool* mac_status,
                     bool* running, 
                     MacCtx_t* mac_context, 
                     queue* in,
                     queue* out,
                     uint32_t* error)
{
    params->mac_status = mac_status;
    params->running = running;
    params->mac_context = mac_context;
    params->in = in;
    params->out = out;
    params->error = error;
}
