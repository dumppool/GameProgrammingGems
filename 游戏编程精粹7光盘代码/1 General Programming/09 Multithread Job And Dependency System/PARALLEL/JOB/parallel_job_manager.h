#ifndef __PARALLEL_JOB_MANAGER__
    #define __PARALLEL_JOB_MANAGER__

    // -- LOCAL

    // .. REFERENCES

	#include "counted_object.h"
    #include "counted_ref_to_.h"
    #include "parallel_atomicity.h"
    #include "parallel_job.h"
    #include "parallel_job_function.h"
    #include "parallel_job_handle.h"
    #include "parallel_job_priority.h"
    #include "parallel_job_scheduler.h"
    #include "parallel_job_working_thread.h"
    #include "parallel_section.h"
    #include <vector>
    #include <list>

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_JOB_MANAGER :
        public COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_JOB_MANAGER(
            VOID
            );

        // ~~

        virtual ~PARALLEL_JOB_MANAGER(
            VOID
            );

        // ~~

        std::vector<COUNTED_REF_TO_<PARALLEL_JOB_WORKING_THREAD> > & GetWorkingThreadTable(
            void
            )
        {
            return WorkingThreadTable;
        }

        // .. INQUIRIES

        BOOL IsJobFinished(
            PARALLEL_JOB_HANDLE job
            ) const;

        // .. OPERATIONS

        VOID Initialize(
            VOID
            );

        // ~~

        PARALLEL_JOB_HANDLE CreateJob(
            PARALLEL_JOB_FUNCTION function,
            VOID * context,
            PARALLEL_JOB_PRIORITY priority = PARALLEL_JOB_PRIORITY_Default
            );

        // ~~

        PARALLEL_JOB_HANDLE CreateAndScheduleJob(
            PARALLEL_JOB_FUNCTION function,
            VOID * context,
            PARALLEL_JOB_PRIORITY priority = PARALLEL_JOB_PRIORITY_Default
            );

        // ~~

        void ScheduleJob(
            PARALLEL_JOB_HANDLE job_handle
            );

        // ~~

        bool GetNextJob(
            PARALLEL_JOB & job
            );

        // ~~

        void WakeUpScheduler(
            void
            )
        {
            Scheduler.GetWaitForJobEvent().Pulse();
        }

        static PARALLEL_JOB_MANAGER & GetInstance(
            void
            )
        {
            if( Instance == 0 )
            {
                Instance = new PARALLEL_JOB_MANAGER();
            }

            return *Instance;
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_JOB_MANAGER(
            const PARALLEL_JOB_MANAGER & other
            );

        // .. OPERATORS

        PARALLEL_JOB_MANAGER & operator=(
            const PARALLEL_JOB_MANAGER & other
            );

        // .. ATTRIBUTES

        // :TODO: Change this with multithreadable list

        PARALLEL_JOB_SCHEDULER
            Scheduler;
        std::vector<COUNTED_REF_TO_<PARALLEL_JOB_WORKING_THREAD> >
            WorkingThreadTable;
        std::list<PARALLEL_JOB>
            QueueJobList;
        std::list<PARALLEL_JOB>
            CreatedJobList;
        int
            NextJobIndex;
        mutable PARALLEL_ATOMICITY
            Atomicity;

        static PARALLEL_JOB_MANAGER
            * Instance;
    };

    // .. FUNCTIONS

    #define PARALLEL_JOB_MANAGER_CreateJob \
        PARALLEL_JOB_MANAGER::GetInstance().CreateJob

    // ~~

    #define PARALLEL_JOB_MANAGER_ScheduleJob \
        PARALLEL_JOB_MANAGER::GetInstance().ScheduleJob
#endif
