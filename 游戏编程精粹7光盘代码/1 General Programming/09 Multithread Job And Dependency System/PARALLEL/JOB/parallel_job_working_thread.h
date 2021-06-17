#ifndef __PARALLEL_JOB_WORKING_THREAD__
    #define __PARALLEL_JOB_WORKING_THREAD__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_event.h"
    #include "parallel_job.h"
    #include "parallel_job_type.h"
    #include "parallel_thread.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_JOB_WORKING_THREAD :
        public PARALLEL_THREAD
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_JOB_WORKING_THREAD(
            VOID
            );

        // ~~

        virtual ~PARALLEL_JOB_WORKING_THREAD(
            VOID
            );

        // .. ACCESSORS

        VOID SetAssignedJob(
            const PARALLEL_JOB & assigned_job
            )
        {
            AssignedJob = assigned_job;
        }

        // ~~

        const PARALLEL_JOB & GetAssignedJob(
            VOID
            ) const
        {
            return AssignedJob;
        }

        // ~~

        VOID SetAssignedJobType(
            const PARALLEL_JOB_TYPE & assigned_job_type
            )
        {
            AssignedJobType = assigned_job_type;
        }

        // ~~

        const PARALLEL_JOB_TYPE & GetAssignedJobType(
            VOID
            ) const
        {
            return AssignedJobType;
        }

        // ~~

        PARALLEL_EVENT & GetDataIsReadyEvent(
            VOID
            )
        {
            return DataIsReadyEvent;
        }

        // ~~

        PARALLEL_EVENT & GetWaitingForDataEvent(
            VOID
            )
        {
            return WaitingForDataEvent;
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

        PARALLEL_JOB_WORKING_THREAD(
            const PARALLEL_JOB_WORKING_THREAD & other
            );

        // .. OPERATORS

        PARALLEL_JOB_WORKING_THREAD & operator=(
            const PARALLEL_JOB_WORKING_THREAD & other
            );

        // .. ATTRIBUTES

        PARALLEL_JOB
            AssignedJob;
        PARALLEL_JOB_TYPE
            AssignedJobType;
        PARALLEL_EVENT
            DataIsReadyEvent,
            WaitingForDataEvent;
    };
#endif
