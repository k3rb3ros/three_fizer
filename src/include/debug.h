#ifndef DEBUG_H

#define DEBUG_H

    #ifdef DEBUG
        #define pdebug(format, ...) printf(format, ## __VA_ARGS__)
    #endif
    #ifndef DEBUG
        #define pdebug(...) void(0)
    #endif 

#endif
