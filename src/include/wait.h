#ifndef WAIT_H
#define WAIT_H

#include <time.h>

/* This header file defines the timespec structure that nanosleep uses 
 * It is used to determine how long how long spawned threads will wait if the can't get a lock on a shared resource
 */

const static struct timespec wait_interval =
{
    .tv_sec = 0,
    .tv_nsec = 33000  
};

#endif /* end WAIT_H */
