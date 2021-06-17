#include "parallel_job_scheduler.h"

// -- LOCAL

// .. REFERENCES

#include "parallel.h"
#include "parallel_handle.h"
#include "parallel_job_manager.h"
#include "parallel_job_working_thread.h"
#include "parallel_thread.h"
#include "parallel_thread_state.h"

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_JOB_SCHEDULER::PARALLEL_JOB_SCHEDULER(
    VOID
    ) :
    PARALLEL_THREAD()
{
    WaitForJobEvent.Create( 0, false );
}

// ~~

PARALLEL_JOB_SCHEDULER::~PARALLEL_JOB_SCHEDULER(
    VOID
    )
{
}

// -- PROTECTED

// .. OPERATIONS

INT PARALLEL_JOB_SCHEDULER::Execute(
    VOID
    )
{
    // Assume Working thread were already created.

    std::vector<PARALLEL_HANDLE>
        wait_for_data_table;
    std::vector<COUNTED_REF_TO_<PARALLEL_JOB_WORKING_THREAD> >
        & working_thread_table = PARALLEL_JOB_MANAGER::GetInstance().GetWorkingThreadTable();
    unsigned int
        working_thread_index;

    wait_for_data_table.resize( working_thread_table.size() );

    for( working_thread_index = 0; working_thread_index < working_thread_table.size(); ++ working_thread_index )
    {
        wait_for_data_table[ working_thread_index ]
            = working_thread_table[ working_thread_index ]->GetWaitingForDataEvent();
    }

    while ( GetState() != PARALLEL_THREAD_STATE_Stopping )
    {
        int
            object_index;
        PARALLEL_JOB
            next_job;

        object_index
            = PARALLEL_WaitMultipleObjects(
                  wait_for_data_table.size(),
                  &wait_for_data_table[ 0 ],
                  false,
                  INFINITE
                  );

        object_index -= WAIT_OBJECT_0;

        if( PARALLEL_JOB_MANAGER::GetInstance().GetNextJob( next_job ) )
        {
            working_thread_table[ object_index ]->SetAssignedJob( next_job );
            working_thread_table[ object_index ]->GetWaitingForDataEvent().Reset();
            working_thread_table[ object_index ]->GetDataIsReadyEvent().Signal();
        }
        else
        {
            WaitForJobEvent.Wait();
        }
    }

    return 0;
}
