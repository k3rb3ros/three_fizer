#include "include/cryptoThread.h"

/*
* This function assumes the header has been tested to be valid but is still encrypted*/
void* decryptQueue(void* parameters)
{
    pdebug("decryptQueue()\n");
    bool decrypted = false;
    bool header = true;
    chunk* decrypt_chunk = NULL;
    cryptParams* params = parameters;
    uint64_t* chain = NULL;

    chain = calloc((params->tf_key->stateSize/64), sizeof(uint64_t));

    if(chain == NULL) //check that calloc succeeded
    {
        pdebug("$$$ Error allocating memory for chain buffer $$$\n");
        *(params->error) = MEMORY_ALLOCATION_FAIL;
        return NULL;
    }

    while(*(params->running) && *(params->error) == 0)
    {
        //check for termination conditions 
        if(decrypt_chunk != NULL && decrypt_chunk->action == DONE)
        {
            pdebug("$$$ decryptQueue() loop terminating $$$\n");
            destroyChunk(decrypt_chunk);
            break;
        }

        //get the next chunk
        if(decrypt_chunk == NULL)
        {
            pthread_mutex_lock(params->in_mutex);
            if(front(params->in) != NULL)
            {
                 decrypt_chunk = front(params->in);
                 if(decrypt_chunk != NULL) { deque(params->in); }
                 decrypted = false;
                 pdebug("$$$ Dequing chunk of size %lu $$$\n", decrypt_chunk->data_size);
            }
            pthread_mutex_unlock(params->in_mutex);
        }

        /*
        * Get the chain block from the encrypted header then strip it out
        */
        if(header && decrypt_chunk != NULL)
        {
            //getChainInBuffer(decrypt_chunk->data, chain, 2, params->tf_key->stateSize);
            destroyChunk(decrypt_chunk);
            decrypt_chunk = NULL;
            header = false;
            pdebug("$$$ Stripping out header $$$\n");
        }
        else if(decrypt_chunk != NULL && !decrypted) //decrypt the chunk
        {
            uint64_t num_blocks = getNumBlocks(decrypt_chunk->data_size,
                                              (uint32_t)params->tf_key->stateSize);
            //getChainInBuffer(decrypt_chunk->data, chain, num_blocks, params->tf_key->stateSize); 
	    decryptInPlace(params->tf_key, chain, decrypt_chunk->data, num_blocks);
            decrypted = true;
            pdebug("$$$ Decrypting chunk of size %lu $$$\n", decrypt_chunk->data_size);
        }
        
        if(decrypt_chunk != NULL && decrypted) //attempt to queue the chunk
        {
            pthread_mutex_lock(params->out_mutex);
            if(!queueIsFull(params->out))
            {
                if(enque(decrypt_chunk, params->out))
                {
                    pdebug("$$$ Queueing chunk of size %lu $$$\n", decrypt_chunk->data_size);
                    decrypt_chunk = NULL;
                    decrypted = false;
                }
            }
            pthread_mutex_unlock(params->out_mutex);
        } //end queue operation
	pd2("DecryptQueue() Tick\n");
    } //end while loop

    //queue Done flag
    while(*(params->running) && *(params->error) == 0 && queueIsFull(params->out)) { ; } //spin unitl queue is free
    pthread_mutex_lock(params->out_mutex);
    if(!queueDone(params->out))
    {
        pdebug("$$$ Error queueing done $$$\n");
        *(params->error) = QUEUE_OPERATION_FAIL;
        return NULL;
    }
    pthread_mutex_unlock(params->out_mutex);

    pdebug("$$$ Done queued $$$ \n");
    pdebug("$$$ Decryption complete $$$\n");

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
    bool encrypted = false; //a flag to protect a chunk from getting encrpted multiple times
    cryptParams* params = parameters;
    chunk* encrypt_chunk = NULL;
    uint64_t* chain = NULL;
    
    chain = calloc(params->tf_key->stateSize/64, sizeof(uint64_t));
    if(chain == NULL) //check that calloc succeeded
    {
        *(params->error) = MEMORY_ALLOCATION_FAIL;
        return NULL;
    }

    while(*(params->running) && *(params->error) == 0) 
    {
        if(encrypt_chunk == NULL)
        {
            pthread_mutex_lock(params->in_mutex);
            if(front(params->in) != NULL)
            {
                encrypt_chunk = front(params->in);
                if(encrypt_chunk != NULL) 
		{ 
		    encrypted = false; //set the encrypted flag
		    deque(params->in); 
		}
            }
            pthread_mutex_unlock(params->in_mutex);
        }
         
        if(encrypt_chunk != NULL && encrypt_chunk->action == DONE)
        {
            pdebug("$$$ encryptQueue() terminating loop $$$\n");
            destroyChunk(encrypt_chunk);
            break;
        }

        if(first_chunk && encrypt_chunk != NULL) //assume the first chunk is the header
        {
            pdebug("$$$ Encrypting header of size %lu $$$\n", encrypt_chunk->data_size);
            if(!encryptHeader(params->tf_key, encrypt_chunk->data))
            { //if we failed to encrypt the header
                 pdebug("$$$ Failed to encrypt header $$$\n");
                 destroyChunk(encrypt_chunk);
                 *(params->error) = CIPHER_OPERATION_FAIL; //set the error flag
                 break; //break out
            }
            //save the chain of cipher text in the next chunk
            getChainInBuffer(encrypt_chunk->data, chain, 2, params->tf_key->stateSize);
            first_chunk = false;
        }
        else if(!first_chunk && encrypt_chunk != NULL && !encrypted)
        {
            uint64_t num_blocks = getNumBlocks(encrypt_chunk->data_size,
                                              (uint32_t)params->tf_key->stateSize);
            encryptInPlace(params->tf_key, chain, encrypt_chunk->data, num_blocks);
            getChainInBuffer(encrypt_chunk->data, chain, num_blocks, params->tf_key->stateSize);
	    encrypted = true;
        }
         
        if(encrypt_chunk != NULL && !queueIsFull(params->out)) //attempt to queue the last encrypted chunk
        {
            encrypt_chunk->action = GEN_MAC; //change the next queued action
            //on success clear the chunk ptr so the next operation can happen
            pthread_mutex_lock(params->out_mutex);
            if(enque(encrypt_chunk, params->out))
            {
                pdebug("$$$ Queing encrypted chunk of size %lu $$$\n", encrypt_chunk->data_size); 
                encrypt_chunk = NULL; 
            }
            pthread_mutex_unlock(params->out_mutex);
        } //end queue operation
	pd2("EncryptQueue() Tick\n");
    } //end while loop
    
    //queue Done flag
    while(queueIsFull(params->out));
    pthread_mutex_lock(params->out_mutex);
    if(!queueDone(params->out))
    {
        pdebug("Error queueing done\n");
        *(params->error) = QUEUE_OPERATION_FAIL;
        if(chain != NULL) { free(chain); }

        return NULL;
    }
    pthread_mutex_unlock(params->out_mutex);
    pdebug("$$$ Done queued $$$ \n");

    if(chain != NULL) { free(chain); }
 
    return NULL;
} //end encryptQueue() 

inline void setUpCryptoParams(cryptParams* params,
                              const arguments* args,
                              bool* running,
                              ThreefishKey_t* tf_key,
                              pthread_mutex_t* in_mutex,
                              pthread_mutex_t* out_mutex,
                              queue* in,
                              queue* out,
                              int32_t* error)
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
