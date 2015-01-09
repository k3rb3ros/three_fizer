#ifndef DEBUG_H

#define DEBUG_H

    #ifdef DEBUG
        #define pdebug(format, ...) printf(format, ##__VA_ARGS__)
    #endif
    #ifndef DEBUG
        #define pdebug(...) do {} while(0)
    #endif

    #ifdef LV2_DEBUG
        #define pd2(format, ...) printf(format, ##__VA_ARGS__)
    #endif
    #ifndef LV2_DEBUG
        #define pd2(...) do {} while(0)
    #endif
    

#endif
