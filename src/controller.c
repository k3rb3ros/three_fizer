#include "include/controller.h"

int32_t runThreefizer(const arguments* args)
{
    bool mac_status = true;
    bool running = true;
    bool threads_active = false;
    bool valid = false;
    uint32_t error = 0;
    const uint8_t* temp_file_name = hash((uint8_t*)args->argz, 10, args->state_size); 
    const uint64_t block_byte_size = (args->state_size/8);
    pthread_t read_thread;
    pthread_t crypto_thread;
    pthread_t mac_thread;
    pthread_t write_thread;
    pthread_mutex_t crypto_mutex;
    pthread_mutex_t mac_mutex;
    pthread_mutex_t write_mutex;
    queue* crypto_queue = createQueue(QUE_SIZE);
    queue* mac_queue = createQueue(QUE_SIZE);
    queue* write_queue = createQueue(QUE_SIZE);
    cryptParams crypto_params;
    macParams mac_params;
    readParams read_params;
    writeParams write_params;
    MacCtx_t mac_context;
    static int32_t status = SUCCESS;
    static uint64_t file_size = 0;
    static ThreefishKey_t tf_key; 

    pdebug("Threefizer controller\n");
    pdebug("Arguments { ");
    pdebug("free: %d, encrypt: %d, hash: %d, argz: [%s], argz_len: %zu, State Size: %u, password: [%s], pw_length %lu, file_length %lu }\n", args->free, args->encrypt, args->hash, args->argz, args->argz_len, args->state_size, args->password, args->pw_length, args->file_size);

    //Init mutexes
    pthread_mutex_init(&crypto_mutex, NULL);
    pthread_mutex_init(&mac_mutex, NULL);
    pthread_mutex_init(&write_mutex, NULL);

    handleKeys(args, &tf_key, &mac_context); //generate and initialize the keys
    if(args->encrypt == true && args->file_size > 0)
    {
        setUpCryptoParams(&crypto_params, args, &running, &tf_key, &crypto_mutex, &mac_mutex, crypto_queue, mac_queue, &error);
        setUpMacParams(&mac_params, &mac_status, &running, NULL, &mac_context, &mac_mutex, &write_mutex, mac_queue, write_queue, NULL, &error, NULL); 
        setUpReadParams(&read_params, args, &running, &crypto_mutex, crypto_queue, &error);
        setUpWriteParams(&write_params, args, &running, NULL, &write_mutex, write_queue, temp_file_name, &error, NULL);
        queueHeader(args, crypto_queue);
        threads_active = true;
        pthread_create(&read_thread, NULL, queueFile, &read_params);
        pthread_create(&crypto_thread, NULL, encryptQueue, &crypto_params);
        pthread_create(&mac_thread, NULL, generateMAC, &mac_params);
        pthread_create(&write_thread, NULL, asyncWrite, &write_params);
    }
    else if(args->file_size == 0)
    {
        perror("File to encrypt is empty aborting\n");
        status = FILE_TOO_SMALL;
    }
    else //decrypt
    {
        setUpReadParams(&read_params, args, &running, &crypto_mutex, mac_queue, &error);
        setUpMacParams(&mac_params, &mac_status, &running, &valid, &mac_context, &mac_mutex, &crypto_mutex, mac_queue, crypto_queue, &tf_key, &error, &file_size); 
        setUpCryptoParams(&crypto_params, args, &running, &tf_key, &crypto_mutex, &write_mutex, crypto_queue, write_queue, &error);
        setUpWriteParams(&write_params, args, &running, &valid, &write_mutex, write_queue, temp_file_name, &error, &file_size);
        threads_active = true;
        pthread_create(&read_thread, NULL, queueFile, &read_params);
        pthread_create(&mac_thread, NULL, authenticateMAC, &mac_params);
        pthread_create(&crypto_thread, NULL, decryptQueue, &crypto_params);
        pthread_create(&write_thread, NULL, asyncWrite, &write_params);
    }

    //free all allocated resources
    if(threads_active) 
    { 
        pthread_join(read_thread, NULL);
        pthread_join(crypto_thread, NULL);
        pthread_join(mac_thread, NULL);
        pthread_join(write_thread, NULL);
    }
    if(error != 0) { status = error; } //return the error if 1 occured
    //free alloated resources
    pthread_mutex_destroy(&crypto_mutex);
    pthread_mutex_destroy(&mac_mutex);
    pthread_mutex_destroy(&write_mutex);
    destroyQueue(crypto_queue);
    destroyQueue(mac_queue);
    destroyQueue(write_queue);
    if(temp_file_name != NULL) { free((void*) temp_file_name); }

    pdebug("Threefizer operation complete\n");

    return status;
}
