#include "include/mac.h"

/*
 * Checks the MAC in the expected chunk and compares it with the generated MAC
 * byte by byte returns true if the MAC's match
 */

bool checkMAC(chunk* expected, uint8_t* generated, uint64_t mac_byte_size)
{
     if(expected->data_size != mac_byte_size) { return false; } //size check

     uint8_t* check = expected->data;

     for(uint64_t i=0; i<mac_byte_size; i++)
     {
         if(generated[i] != check[i]) { return false; }
     }
     return true;
}

/*
 *This function iterates through the queue passed to in and updates a SkeinMAC
 * before moving each chunk to the out que.
 * When the DONE chunk is encountered in the queue the final MAC is returned.
 * Done and the MAC are not queued as futher action or inaction may be required
 * based on what the queue is.
 */
uint64_t* genMAC(MacCtx_t* mac_context, queue* in, queue* out)
{
    pdebug("genMAC()\n");

    chunk* update_chunk = NULL;
    uint64_t* mac = NULL;

    //iterate through the queue and generate a MAC for everything in it
    while(in != NULL && front(in)->action != DONE && front(in)->action != MAC)
    {
        //check if we are free to MAC a chunk if so do it
        if(update_chunk == NULL)
        {
             update_chunk = front(in);
             deque(in); //pop the 
             if(update_chunk->action != GEN_MAC) //sanity check
             {
                 perror("Bad data in mac queue aborting operation\n");
                 destroyChunk(update_chunk);
                 return NULL;
             }

             update_chunk->action = mac_context->out_action; //set the chunk action to WRITE
             skeinUpdate(mac_context->skein_context_ptr, (const uint8_t*)update_chunk->data, update_chunk->data_size);   
        }

        if(update_chunk != NULL && enque(update_chunk, out) == true) //attempt to queue the chunk
        {
            update_chunk = NULL; //Set mac chunk to NULL so the next chunk will be maced
        }
        //otherwise spin and wait for the queue to empty
    }

    if(in != NULL && front(in)->action == DONE)
    {
         mac = calloc(mac_context->digest_byte_size, sizeof(uint8_t));
         skeinFinal(mac_context->skein_context_ptr, mac);
    } 
    
    return mac;
}

//Initialize the mac (do all the Skein initing here)
void InitMacCtx(MacCtx_t* context,
                  uint64_t* mac_key,
                  uint64_t digest_byte_size,
                  SkeinSize_t state_size)
{
    
    const uint16_t key_byte_size = (const uint16_t)state_size/8;

    context->skein_context_ptr = &(context->skein_context);
    context->out_action = DONE;
    context->mac_key = mac_key;
    context->digest_byte_size = digest_byte_size;
    context->key_size = (state_size/8);
    
    skeinCtxPrepare(context->skein_context_ptr, state_size);

    skeinMacInit(context->skein_context_ptr, 
                 (uint8_t*)mac_key, 
                 key_byte_size, 
                 (digest_byte_size*8)); //digest size is in bits in Skein Mac Init
}
