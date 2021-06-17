// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel_dependency_identifier.h"
#include "parallel_job.h"
#include "parallel_job_function.h"
#include "parallel_job_handle.h"
#include "parallel_job_manager.h"
#include <assert.h>

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_JOB_MANAGER::PARALLEL_JOB_MANAGER(
    VOID
    ) :
    COUNTED_OBJECT(),
    QueueJobList(),
    NextJobIndex( 0 )
{
}

// ~~

PARALLEL_JOB_MANAGER::~PARALLEL_JOB_MANAGER(
    VOID
    )
{
}

// .. OPERATIONS

VOID PARALLEL_JOB_MANAGER::Initialize(
    VOID
    )
{

}

// ~~

PARALLEL_JOB_HANDLE PARALLEL_JOB_MANAGER::CreateJob(
    PARALLEL_JOB_FUNCTION function,
    VOID * context,
    PARALLEL_JOB_PRIORITY priority
    )
{
    PARALLEL_JOB
        job;

    job.SetFunction( function );
    job.SetContext( context );
    job.Execute();

    return PARALLEL_JOB_HANDLE();
}

// ~~

PARALLEL_JOB_HANDLE PARALLEL_JOB_MANAGER::CreateAndScheduleJob(
    PARALLEL_JOB_FUNCTION function,
    VOID * context,
    PARALLEL_JOB_PRIORITY priority
    )
{
    PARALLEL_JOB_HANDLE
        job_handle;

    job_handle = CreateJob( function, context );
    ScheduleJob( job_handle );

    return job_handle;
}

// ~~

void PARALLEL_JOB_MANAGER::ScheduleJob(
    PARALLEL_JOB_HANDLE job_handle
    )
{
}

// ~~

PARALLEL_DEPENDENCY_IDENTIFIER GetJobDependencyIdentifier(
    VOID
    )
{
    PARALLEL_DEPENDENCY_IDENTIFIER
        return_value;

    return return_value;
}

// ~~

bool PARALLEL_JOB_MANAGER::GetNextJob(
    PARALLEL_JOB & job
    )
{
    assert( "Not used in single threaded" );

    return false;
}
