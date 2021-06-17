#ifndef __PARALLEL_JOB_SCHEDULER__
    #define __PARALLEL_JOB_SCHEDULER__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_event.h"
    #include "parallel_thread.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_JOB_SCHEDULER :
        public PARALLEL_THREAD
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_JOB_SCHEDULER(
            VOID
            );

        // ~~

        virtual ~PARALLEL_JOB_SCHEDULER(
            VOID
            );

        // .. ACCESSORS

        PARALLEL_EVENT & GetWaitForJobEvent(
            VOID
            )
        {
            return WaitForJobEvent;
        }

        // -- PROTECTED

    protected :

        // .. OPERATIONS

        virtual INT Execute(
            VOID
            );

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_JOB_SCHEDULER(
            const PARALLEL_JOB_SCHEDULER & other
            );

        // .. OPERATORS

        PARALLEL_JOB_SCHEDULER & operator=(
            const PARALLEL_JOB_SCHEDULER & other
            );

        // .. ATTRIBUTES

        PARALLEL_EVENT
            WaitForJobEvent;
    };
#endif
