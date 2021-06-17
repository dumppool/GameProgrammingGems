#include "parallel_job_manager.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel.h"
#include "parallel_dependency_identifier.h"
#include "parallel_dependency_manager.h"
#include "parallel_job.h"
#include "parallel_job_function.h"
#include "parallel_job_handle.h"
#include "parallel_job_working_thread.h"
#include "parallel_section.h"
#include <algorithm>

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_JOB_MANAGER::PARALLEL_JOB_MANAGER(
    void
    ) :
    COUNTED_OBJECT(),
    QueueJobList(),
    NextJobIndex( 0 )
{

}

// ~~

PARALLEL_JOB_MANAGER::~PARALLEL_JOB_MANAGER(
    void
    )
{
    size_t
        thread_index;

    Scheduler.StopAndWait();

    for( thread_index = 0; thread_index < WorkingThreadTable.size(); ++thread_index )
    {
        WorkingThreadTable[ thread_index ]->StopAndWait();
    }

    WorkingThreadTable.clear();
}

// .. OPERATIONS

void PARALLEL_JOB_MANAGER::Initialize(
    void
    )
{
    int
        thread_index;

    WorkingThreadTable.resize( PARALLEL_GetProcessorCount() - 1 );

    for( thread_index = 0; thread_index < int(WorkingThreadTable.size()); ++thread_index )
    {
        WorkingThreadTable[ thread_index ] = new PARALLEL_JOB_WORKING_THREAD;
        WorkingThreadTable[ thread_index ]->Start( thread_index + 1 );
    }

    Scheduler.Start();
}

// ~~

PARALLEL_JOB_HANDLE PARALLEL_JOB_MANAGER::CreateJob(
    PARALLEL_JOB_FUNCTION function,
    void * context,
    PARALLEL_JOB_PRIORITY priority
    )
{
    PARALLEL_JOB
        job;

    PARALLEL_SECTION_Begin( Atomicity )
    {
        job.SetJobHandle( 
            PARALLEL_JOB_HANDLE(
                NextJobIndex,
                PARALLEL_DEPENDENCY_MANAGER::GetInstance().CreateJobDependencyEntry()
                )
            );
        ++NextJobIndex;

        job.SetFunction( function );
        job.SetContext( context );
        job.SetPriority( priority );

        CreatedJobList.push_back( job );

        return job.GetJobHandle();
    }

    PARALLEL_SECTION_End()
}

// ~~

PARALLEL_JOB_HANDLE PARALLEL_JOB_MANAGER::CreateAndScheduleJob(
    PARALLEL_JOB_FUNCTION function,
    void * context,
    PARALLEL_JOB_PRIORITY priority
    )
{
    PARALLEL_JOB_HANDLE
        job_handle;

    job_handle = CreateJob( function, context, priority );
    ScheduleJob( job_handle );

    return job_handle;
}

// ~~

void PARALLEL_JOB_MANAGER::ScheduleJob(
    PARALLEL_JOB_HANDLE job_handle
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        std::list<PARALLEL_JOB>::iterator
            job_iterator;

        for( job_iterator = CreatedJobList.begin(); job_iterator != CreatedJobList.end(); ++job_iterator )
        {
            if ( job_iterator->GetJobHandle() == job_handle )
            {
                QueueJobList.push_back( *job_iterator );
				QueueJobList.sort();
                job_iterator = CreatedJobList.erase( job_iterator );

                Scheduler.GetWaitForJobEvent().Signal();

                return;
            }
        }

        assert( false && "Invalid job handle" );
    }

    PARALLEL_SECTION_End()
}

// ~~

bool PARALLEL_JOB_MANAGER::GetNextJob(
    PARALLEL_JOB & job
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        std::list<PARALLEL_JOB>::iterator
            job_iterator;
        PARALLEL_JOB
            current_job;
        PARALLEL_DEPENDENCY_MANAGER
            & manager = PARALLEL_DEPENDENCY_MANAGER::GetInstance();

        for( job_iterator = QueueJobList.begin(); job_iterator != QueueJobList.end(); ++ job_iterator )
        {
           if( manager.IsDependencyMet( (*job_iterator).GetJobHandle() ) )
           {
               job = *job_iterator;
               QueueJobList.erase( job_iterator );
               return true;
           }
        }

        return false;
    }
    PARALLEL_SECTION_End()
}
