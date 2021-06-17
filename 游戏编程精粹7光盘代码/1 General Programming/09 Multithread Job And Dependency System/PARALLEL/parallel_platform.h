#ifndef __PARALLEL_PLATFORM__
    #define __PARALLEL_PLATFORM__

    // -- LOCAL

    // .. REFERENCES

    // -- GLOBAL

    // .. CONSTANTS
    
    #ifdef __OPTION_MULTITHREAD__
        #define PARALLEL_PLATFORM_Multithread 1
    #else
        #define PARALLEL_PLATFORM_Singlethread 1
    #endif

    // ~~

    #ifndef PARALLEL_PLATFORM_Multithread
        #define PARALLEL_PLATFORM_Multithread 0
    #endif

    // ~~

    #ifndef PARALLEL_PLATFORM_Singlethread
        #define PARALLEL_PLATFORM_Singlethread 0
    #endif


#endif
