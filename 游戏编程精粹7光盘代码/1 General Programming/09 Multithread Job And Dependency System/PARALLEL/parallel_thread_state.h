#ifndef __PARALLEL_THREAD_STATE__
    #define __PARALLEL_THREAD_STATE__

    // -- LOCAL

    // .. REFERENCES

    // -- GLOBAL

    // .. TYPES

    enum PARALLEL_THREAD_STATE
    {
        PARALLEL_THREAD_STATE_Constructed,
        PARALLEL_THREAD_STATE_Starting,
        PARALLEL_THREAD_STATE_Running,
        PARALLEL_THREAD_STATE_Suspended,
        PARALLEL_THREAD_STATE_Stopping,
        PARALLEL_THREAD_STATE_Finished
    };
#endif
