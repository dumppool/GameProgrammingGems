#ifndef __PARALLEL_JOB_PRIORITY__
    #define __PARALLEL_JOB_PRIORITY__

    // -- LOCAL

    // .. REFERENCES

    // -- GLOBAL

    // .. TYPES

    enum PARALLEL_JOB_PRIORITY
    {
        PARALLEL_JOB_PRIORITY_High = 0,
        PARALLEL_JOB_PRIORITY_Normal,
        PARALLEL_JOB_PRIORITY_Low,
        PARALLEL_JOB_PRIORITY_Idle,
        PARALLEL_JOB_PRIORITY_Count,
        PARALLEL_JOB_PRIORITY_Default = PARALLEL_JOB_PRIORITY_Normal,
        PARALLEL_JOB_PRIORITY_None = -1
    };
#endif
