#ifndef __PARALLEL_THREAD_IDENTIFIER__
    #define __PARALLEL_THREAD_IDENTIFIER__

    // -- LOCAL

    // .. REFERENCES

    // -- GLOBAL

    // .. TYPES

    #if PLATFORM_Xbox360
        typedef DWORD
            PARALLEL_THREAD_IDENTIFIER;
    #else
        typedef UINT
            PARALLEL_THREAD_IDENTIFIER;
    #endif

#endif
