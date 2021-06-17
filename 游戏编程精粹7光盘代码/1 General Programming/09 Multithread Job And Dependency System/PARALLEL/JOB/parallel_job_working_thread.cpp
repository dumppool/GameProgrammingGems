#include "parallel_job_working_thread.h"

// -- LOCAL

// .. REFERENCES

#include "parallel.h"
#include "parallel_thread.h"
#include "parallel_thread_state.h"
#include "parallel_dependency_manager.h"
#include "parallel_job_manager.h"
#include <stdio.h>

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_JOB_WORKING_THREAD::PARALLEL_JOB_WORKING_THREAD(
    VOID
    ) :
    PARALLEL_THREAD()
{
    char
        data_is_ready_event[ 256 ],
        waiting_for_data_event[ 256 ];

    sprintf_s( data_is_ready_event, "data_is_ready_event_%p", this );
    sprintf_s( waiting_for_data_event, "waiting_for_data_event_%p", this );

    DataIsReadyEvent.Create( data_is_ready_event, false );
    WaitingForDataEvent.Create( waiting_for_data_event, true, true );
}

// ~~

PARALLEL_JOB_WORKING_THREAD::~PARALLEL_JOB_WORKING_THREAD(
    VOID
    )
{
}

// -- PROTECTED

// .. OPERATIONS

INT PARALLEL_JOB_WORKING_THREAD::Execute(
    VOID
    )
{
    // :TODO: This doesn't really allowed clean exit.

    while ( GetState() != PARALLEL_THREAD_STATE_Stopping )
    {
        PARALLEL_WaitObject( DataIsReadyEvent, INFINITE );

        AssignedJob.Execute();

        PARALLEL_DEPENDENCY_MANAGER::GetInstance().SetJobIsFinished( AssignedJob.GetJobHandle() );
        PARALLEL_JOB_MANAGER::GetInstance().WakeUpScheduler();
        
        WaitingForDataEvent.Signal();
    }

    return 0;
}
