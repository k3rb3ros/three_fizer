#include "include/cryptoThread.h"

void* decryptQueue(void* parameters)
{
    cryptParams* params = parameters;
    return NULL;
}

void* encryptQueue(void* parameters)
{
    pdebug("encryptQueue()\n");
    bool first_chunk = true;
    chunk* encrypt_chunk = NULL;
    cryptParams* params = parameters;
    uint64_t* chain = NULL;

    while(*(params->running) && *(params->error) == 0 && 
          front(params->in) != NULL && front(params->in)->action != DONE)
    {
         if(encrypt_chunk == NULL)
         {
              //get the top chunk in the queue
              encrypt_chunk = front(params->in);
              deque(params->in); //pop it off the queue

              if(first_chunk) //assume the first chunk is the header
              {
                  pdebug("Encrypting header of size %lu\n", encrypt_chunk->data_size);
                  if(!encryptHeader(params->tf_key, encrypt_chunk->data))
                  { //if we failed to encrypt the header
                      destroyChunk(encrypt_chunk);
                      *(params->error) = true; //set the error flag
                      break; //break out
                  }
                  //save the chain of cipher text in the next chunk
                  chain = getChainInPlace(encrypt_chunk->data, 2, (uint32_t)params->tf_key->stateSize);
                  first_chunk = false;
              }
              else
              {
                  uint64_t num_blocks = getNumBlocks(encrypt_chunk->data_size,
                                                     (uint32_t)params->tf_key->stateSize);
                  encryptInPlace(params->tf_key, chain, encrypt_chunk->data, num_blocks);
                  chain = getChainInPlace(encrypt_chunk->data, num_blocks, (uint32_t) params->tf_key); 
              }
         }
         //spin until the last chunk has been queued
         
         if(encrypt_chunk != NULL && !queueIsFull(params->out)) //attempt to queue the last encrypted chunk
         {
             encrypt_chunk->action = GEN_MAC; //change the next queued action
             //on success clear the chunk ptr so the next operation can happen
             if(enque(encrypt_chunk, params->out))
             {
                 pdebug("Queing chunk of size %lu\n", encrypt_chunk->data_size); 
                 encrypt_chunk = NULL; 
             }
         }
    }
   
    queueDone(params->out);
 
    return NULL;
}

inline void setUpCryptoParams(cryptParams* params,
                             const arguments* args,
                             bool* running,
                             ThreefishKey_t* tf_key,
                             queue* in,
                             queue* out,
                             uint32_t* error)
{
    params->args = args;
    params->running = running;
    params->tf_key = tf_key;
    params->in = in;
    params->out = out;
    params->error = error;
}
