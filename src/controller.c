#include "include/controller.h"

int runThreefizer(arguments* args)
{
    bool mac_status = true;
    bool running = true;
    bool valid = false;
    int32_t error = 0;
    progress_t progress;
    pthread_t read_thread;
    pthread_t crypto_thread;
    pthread_t mac_thread;
    pthread_t write_thread;
    pthread_mutex_t crypto_mutex;
    pthread_mutex_t mac_mutex;
    pthread_mutex_t progress_mutex;
    pthread_mutex_t write_mutex;
    cryptParams crypto_params;
    macParams mac_params;
    readParams read_params;
    writeParams write_params;
    MacCtx_t mac_context;
    static uint64_t new_file_size = 0;
    static ThreefishKey_t tf_key;

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("key_file: %s, password: %s, rename_file: %s, target_file: %s, iv: [%08x] encrypt: %d, free: %d, hash: %d, hash_from_file: %d, legacy_hash: %d, rename: %d, threads_active: %d, state_size: %d, file_size: %d, pw_length: %d",
           args->key_file,
           args->password,
           args->rename_file,
           args->target_file,
           &args->iv,
           args->encrypt,
           args->free,
           args->hash,
           args->hash_from_file,
           args->legacy_hash,
           args->rename,
           args->threads_active,
           args->state_size,
           args->file_size,
           args->pw_length);
    pdebug(" }\n");

    //check the file size
    if((args->encrypt == true && args->file_size == 0) ||
       (args->encrypt == false && !isGreaterThanThreeBlocks(args)))
    {
        return FILE_TOO_SMALL;
    }
    else if(!handleKeys(args, &tf_key, &mac_context)) //generate and initialize keys
    {
        return KEY_GENERATION_FAIL; 
    }

    /* Create the temp file name
     * the temp file name is a partial skein hash of the original file name
     * this should prevent filename collisions in all sane cases. */
    const uint8_t* temp_file_name = binToHex(skeinHash((uint8_t*)args->target_file,
                                             TEMP_FILE_NAME_SIZE,
                                             args->state_size),
                                             TEMP_FILE_NAME_SIZE);

    /*
     * Init resrouces threefizer needs to operate 
     */
    pthread_mutex_init(&crypto_mutex, NULL);
    pthread_mutex_init(&mac_mutex, NULL);
    pthread_mutex_init(&progress_mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);
    queue* crypto_queue = createQueue(QUE_SIZE);
    queue* mac_queue = createQueue(QUE_SIZE);
    queue* write_queue = createQueue(QUE_SIZE);
    setUpProgress(args, &progress, &progress_mutex);

    //Encrypting
    if(args->encrypt == true && args->file_size > 0)
    {
        //set up threads
        setUpReadParams(&read_params,
                        args,
                        &running,
                        &crypto_mutex,
                        crypto_queue,
                        &progress,
                        &error);

        setUpCryptoParams(&crypto_params,
                          args,
                          &running,
                          &tf_key,
                          &crypto_mutex,
                          &mac_mutex,
                          crypto_queue,
                          mac_queue,
                          &progress,
                          &error);

        setUpMacParams(&mac_params,
                       &mac_status,
                       &running,
                       NULL,
                       &mac_context,
                       &mac_mutex,
                       &write_mutex,
                       mac_queue,
                       write_queue,
                       &progress,
                       NULL,
                       &error,
                       NULL);

        setUpWriteParams(&write_params,
                         args,
                         &running,
                         NULL,
                         &write_mutex,
                         write_queue,
                         temp_file_name,
                         &progress,
                         &error,
                         NULL);

        /* generate and queue the Header before we start reading the file so its
         * the first thing in the queue*/
        if(queueHeader(args, crypto_queue) == true)
        {
            //start the threads
            args->threads_active = true;
            pthread_create(&read_thread, NULL, queueFileForEncrypt, &read_params);
            pthread_create(&crypto_thread, NULL, encryptQueue, &crypto_params);
            pthread_create(&mac_thread, NULL, generateMAC, &mac_params);
            pthread_create(&write_thread, NULL, asyncWrite, &write_params);

            /* Display the progress bar if the files is big enough that we have to break
            * it into chunks. */
            if(args->file_size > MAX_CHUNK_SIZE)
            { printProgressBar(&running, &error, &progress); }
        }
        else { return QUEUE_OPERATION_FAIL; } 
    } //end encryption
    else //decryption
    {
        //set up threads
        setUpReadParams(&read_params,
                        args,
                        &running,
                        &mac_mutex,
                        mac_queue,
                        &progress,
                        &error);

        setUpMacParams(&mac_params,
                       &mac_status,
                       &running,
                       &valid,
                       &mac_context,
                       &mac_mutex,
                       &crypto_mutex,
                       mac_queue,
                       crypto_queue,
                       &progress,
                       &tf_key,
                       &error,
                       &new_file_size);

        setUpCryptoParams(&crypto_params,
                          args,
                          &running,
                          &tf_key,
                          &crypto_mutex,
                          &write_mutex,
                          crypto_queue,
                          write_queue,
                          &progress,
                          &error);

        setUpWriteParams(&write_params,
                         args,
                         &running,
                         &valid,
                         &write_mutex,
                         write_queue,
                         temp_file_name,
                         &progress, 
                         &error,
                         &new_file_size);

        //start threads
        args->threads_active = true;
        pthread_create(&read_thread, NULL, queueFileForDecrypt, &read_params);
        pthread_create(&mac_thread, NULL, authenticateMAC, &mac_params);
        pthread_create(&crypto_thread, NULL, decryptQueue, &crypto_params);
        pthread_create(&write_thread, NULL, asyncWrite, &write_params);

        /* Display the progress bar if the files is big enough that we have to break
         * it into chunks. */
        if(args->file_size > MAX_CHUNK_SIZE)
        { printProgressBar(&running, &error, &progress); }
    } //end encryption

    //free all allocated resources
    if(args->threads_active) 
    {   //wait for threads to finish
        pthread_join(read_thread, NULL);
        pthread_join(crypto_thread, NULL);
        pthread_join(mac_thread, NULL);
        pthread_join(write_thread, NULL);
        pdebug("All threads joined\n");
    }

    //free all allocated resources
    pthread_mutex_destroy(&crypto_mutex);
    pthread_mutex_destroy(&mac_mutex);
    pthread_mutex_destroy(&progress_mutex);
    pthread_mutex_destroy(&write_mutex);
    destroyQueue(crypto_queue);
    destroyQueue(mac_queue);
    destroyQueue(write_queue);
    if(args->iv != NULL) { free(args->iv); }

    if(error != 0)
    {
        //if an error occured then delete the temp file
        if(temp_file_name != NULL) { unlink((char*)temp_file_name); }
    }

    if(temp_file_name != NULL) { free((void*)temp_file_name); }

    pdebug("Resources freed\n");

    return error == 0 ? THREEFIZER_SUCCESS : error;
}
