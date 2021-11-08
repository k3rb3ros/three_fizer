#pragma once

#include <time.h>

/* This header file defines the timespec structure that nanosleep uses 
 * It is used to determine how long how long spawned threads will wait if they can't get a
 * lock on a shared resource
 */

const static struct timespec wait_interval =
{
    .tv_sec = 0,
    .tv_nsec = 33000  
};
