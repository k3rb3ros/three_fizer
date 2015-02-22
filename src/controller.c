#include "include/controller.h"

int runThreefizer(const arguments* args)
{
    bool mac_status = true;
    bool running = true;
    bool threads_active = false;
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
    static int32_t status = THREEFIZER_SUCCESS;
    static uint64_t file_size = 0;
    static ThreefishKey_t tf_key;

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, file_path: [%s], file_path_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->target_file, args->file_size, args->state_size, args->password, args->pw_length, args->file_size);

    if(handleKeys(args, &tf_key, &mac_context)) //generate and initialize the keys we cannot continue if this fails
    {
        //Init resources
        //the temp file name is a partial skein hash of the original file name
        const uint8_t* temp_file_name = binToHex(hash((uint8_t*)args->target_file, TEMP_FILE_NAME_SIZE, args->state_size), TEMP_FILE_NAME_SIZE);
        pthread_mutex_init(&crypto_mutex, NULL);
        pthread_mutex_init(&mac_mutex, NULL);
	pthread_mutex_init(&progress_mutex, NULL);
        pthread_mutex_init(&write_mutex, NULL);
        queue* crypto_queue = createQueue(QUE_SIZE);
        queue* mac_queue = createQueue(QUE_SIZE);
        queue* write_queue = createQueue(QUE_SIZE);
	setUpProgress(args, &progress, &progress_mutex);

        if(args->encrypt == true && args->file_size > 0) //encrypt
        {
            //passthrough queues with no encryption
            setUpReadParams(&read_params, args, &running, &crypto_mutex, crypto_queue, &progress, &error);
            setUpCryptoParams(&crypto_params, args, &running, &tf_key, &crypto_mutex, &mac_mutex, crypto_queue, mac_queue, &progress, &error);
            setUpMacParams(&mac_params, &mac_status, &running, NULL, &mac_context, &mac_mutex, &write_mutex, mac_queue, write_queue, &progress, NULL, &error, NULL); 
            setUpWriteParams(&write_params, args, &running, NULL, &write_mutex, write_queue, temp_file_name, &progress, &error, NULL);
            queueHeader(args, crypto_queue); //generate and queue the Header before we start reading the flie so it is the first thing in the queue
            threads_active = true;
            pthread_create(&read_thread, NULL, queueFileForEncrypt, &read_params);
            pthread_create(&crypto_thread, NULL, encryptQueue, &crypto_params);
            pthread_create(&mac_thread, NULL, generateMAC, &mac_params);
            pthread_create(&write_thread, NULL, asyncWrite, &write_params);
	    if(args->file_size > MAX_CHUNK_SIZE) { printProgressBar(&running, &error, &progress); }
        }
        else if(args->file_size == 0)
        {
            perror("File to encrypt is empty aborting\n");
            status = FILE_TOO_SMALL;
        }
        else //decrypt
        {
            setUpReadParams(&read_params, args, &running, &mac_mutex, mac_queue, &progress, &error);
            setUpMacParams(&mac_params, &mac_status, &running, &valid, &mac_context, &mac_mutex, &crypto_mutex, mac_queue, crypto_queue, &progress, &tf_key, &error, &file_size); 
            setUpCryptoParams(&crypto_params, args, &running, &tf_key, &crypto_mutex, &write_mutex, crypto_queue, write_queue, &progress, &error);
            setUpWriteParams(&write_params, args, &running, &valid, &write_mutex, write_queue, temp_file_name, &progress,  &error, &file_size);
            threads_active = true;
            pthread_create(&read_thread, NULL, queueFileForDecrypt, &read_params);
            pthread_create(&mac_thread, NULL, authenticateMAC, &mac_params);
            pthread_create(&crypto_thread, NULL, decryptQueue, &crypto_params);
            pthread_create(&write_thread, NULL, asyncWrite, &write_params);
	    if(args->file_size > MAX_CHUNK_SIZE) { printProgressBar(&running, &error, &progress); }
        }

        //free all allocated resources
        if(threads_active) 
        { 
            pthread_join(read_thread, NULL);
            pthread_join(crypto_thread, NULL);
            pthread_join(mac_thread, NULL);
            pthread_join(write_thread, NULL);
	    pdebug("All threads joined\n");
        }
        if(error != 0)
        {
            status = error;
            if(temp_file_name != NULL) { unlink((char*)temp_file_name); }
        } //return the error if 1 occured

        //free alloated resources
        pthread_mutex_destroy(&crypto_mutex);
        pthread_mutex_destroy(&mac_mutex);
	pthread_mutex_destroy(&progress_mutex);
        pthread_mutex_destroy(&write_mutex);
        destroyQueue(crypto_queue);
        destroyQueue(mac_queue);
        destroyQueue(write_queue);
        if(temp_file_name != NULL) { free((void*)temp_file_name); }
        pdebug("Resources freed\n");
    }
    else { status = KEY_GENERATION_FAIL; }

    return status;
}
