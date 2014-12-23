#include "include/cryptoThread.h"

/*
* This function assumes the header has been tested to be valid but is still encrypted 
*/
void* decryptQueue(void* parameters)
{
    pdebug("decryptQueue()\n");
    bool decrypted = false;
    bool header = true;
    chunk* decrypt_chunk = NULL;
    cryptParams* params = parameters;
    uint64_t* chain = NULL;

    chain = calloc((params->tf_key->stateSize/8), sizeof(uint64_t));
    if(chain == NULL) //check that calloc succeeded
    {
        *(params->error) = MEMORY_ALLOCATION_FAIL;
        return NULL;
    }

    while(*(params->running) && *(params->error) == 0 &&
          (front(params->in) == NULL || front(params->in)->action != DONE))
    {
        if(decrypt_chunk == NULL && front(params->in) != NULL)
        {
             pthread_mutex_lock(params->in_mutex);
             decrypt_chunk = front(params->in);
             deque(params->in);
             pthread_mutex_unlock(params->in_mutex);
             decrypted = false;
        }
        /*
        * Get the chain block from the encrypted header then strip it out
        */
        if(header && decrypt_chunk != NULL)
        {
             getChainInBuffer(decrypt_chunk->data, chain, 2, params->tf_key->stateSize);
             destroyChunk(decrypt_chunk);
             decrypt_chunk = NULL;
        }
        else if(decrypt_chunk != NULL && !decrypted) //decrypt the chunk
        {
            decryptInPlace(params->tf_key, chain, decrypt_chunk->data, decrypt_chunk->data_size);
            getChainInBuffer(decrypt_chunk->data, chain, 2, params->tf_key->stateSize); 
            decrypted = true;
        }
        
        if(decrypt_chunk != NULL && !queueIsFull(params->out)) //attempt to queue the chunk
        {
            pthread_mutex_lock(params->out_mutex);
            enque(decrypt_chunk, params->out);
            pthread_mutex_unlock(params->out_mutex);
            decrypt_chunk = NULL;
            decrypted = false;
        }
    }

    //queue done
    while(queueIsFull(params->out)); 
    pthread_mutex_lock(params->out_mutex);
    queueDone(params->out);
    pthread_mutex_unlock(params->out_mutex);

    if(chain != NULL) { free(chain); } //free allocated resources

    return NULL;
}

/****************************************************************************** 
* This encrypts all queued data passed to 'in' and puts it in the 'out' queue.*
* this function assumes a properly formatted header starting with an IV       *
* is the first thing queued and an empty chunk with the action set to DONE is *
* the last thing queued.                                                      *
*******************************************************************************/

/***********************************************************
* The encrypted file should be written like this           *
* |HEADER|CIPHER_TEXT|MAC|                                 *
* note the MAC operation must include the entire header    *
* which in turn includes the IV                            *
* The mac size is the same as the block size of the cipher *
***********************************************************/

void* encryptQueue(void* parameters)
{
    pdebug("encryptQueue()\n");
    bool first_chunk = true;
    cryptParams* params = parameters;
    chunk* encrypt_chunk = NULL;
    uint64_t* chain = NULL;
    
    chain = calloc(params->tf_key->stateSize/8, sizeof(uint64_t));
    if(chain == NULL) //check that calloc succeeded
    {
        *(params->error) = MEMORY_ALLOCATION_FAIL;
        return NULL;
    }

    while(*(params->running) && *(params->error) == 0 && 
          (front(params->in) == NULL || front(params->in)->action != DONE)) //the queue will return NULL if this thread runs faster then the read thread(very likely)
    {
         if(encrypt_chunk == NULL && front(params->in) != NULL)
         {
              pthread_mutex_lock(params->in_mutex);
              encrypt_chunk = front(params->in);
              deque(params->in); //TODO find out why this is getting called so much
              pthread_mutex_unlock(params->in_mutex);
         }
         
        if(first_chunk && encrypt_chunk != NULL) //assume the first chunk is the header
         {
              pdebug("@@@@Encrypting header of size %lu @@@@\n", encrypt_chunk->data_size);
              if(!encryptHeader(params->tf_key, encrypt_chunk->data))
              { //if we failed to encrypt the header
                   destroyChunk(encrypt_chunk);
                   *(params->error) = CIPHER_OPERATION_FAIL; //set the error flag
                   break; //break out
              }
              //save the chain of cipher text in the next chunk
              getChainInBuffer(encrypt_chunk->data, chain, 2, params->tf_key->stateSize);
              first_chunk = false;
         }
         else if(!first_chunk && encrypt_chunk != NULL)
         {
              uint64_t num_blocks = getNumBlocks(encrypt_chunk->data_size,
                                                 (uint32_t)params->tf_key->stateSize);
              encryptInPlace(params->tf_key, chain, encrypt_chunk->data, num_blocks);
              getChainInBuffer(encrypt_chunk->data, chain, num_blocks, params->tf_key->stateSize);
         }
         
         if(encrypt_chunk != NULL && !queueIsFull(params->out)) //attempt to queue the last encrypted chunk
         {
              encrypt_chunk->action = GEN_MAC; //change the next queued action
              //on success clear the chunk ptr so the next operation can happen
              pthread_mutex_lock(params->out_mutex);
              if(enque(encrypt_chunk, params->out))
              {
                   pdebug("@@@@Queing encrypted chunk of size %lu @@@@\n", encrypt_chunk->data_size); 
                   encrypt_chunk = NULL; 
              }
              pthread_mutex_unlock(params->out_mutex);
         }
    }
    
    while(queueIsFull(params->out)); 
    pthread_mutex_lock(params->out_mutex);
    queueDone(params->out);
    pthread_mutex_unlock(params->out_mutex);

    if(chain != NULL) { free(chain); }
 
    return NULL;
}

inline void setUpCryptoParams(cryptParams* params,
                             const arguments* args,
                             bool* running,
                             ThreefishKey_t* tf_key,
                             pthread_mutex_t* in_mutex,
                             pthread_mutex_t* out_mutex,
                             queue* in,
                             queue* out,
                             uint32_t* error)
{
    params->args = args;
    params->running = running;
    params->tf_key = tf_key;
    params->in_mutex = in_mutex;
    params->out_mutex = out_mutex;
    params->in = in;
    params->out = out;
    params->error = error;
}
