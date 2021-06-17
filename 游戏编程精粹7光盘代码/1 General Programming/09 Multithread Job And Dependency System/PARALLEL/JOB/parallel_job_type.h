#ifndef __PARALLEL_JOB_TYPE__
    #define __PARALLEL_JOB_TYPE__

    // -- LOCAL

    // .. REFERENCES

    // -- GLOBAL

    // .. TYPES

    enum PARALLEL_JOB_TYPE
    {
        PARALLEL_JOB_TYPE_Default = 0,
        PARALLEL_JOB_TYPE_Graphic,
        PARALLEL_JOB_TYPE_Kernel,
        PARALLEL_JOB_TYPE_Animation,
        PARALLEL_JOB_TYPE_Gameplay,
        PARALLEL_JOB_TYPE_Count,
        PARALLEL_JOB_TYPE_None = -1
    };
#endif
