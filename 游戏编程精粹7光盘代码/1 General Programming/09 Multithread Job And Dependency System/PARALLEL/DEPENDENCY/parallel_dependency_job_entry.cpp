#include "parallel_dependency_job_entry.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel_dependency_entry.h"

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_DEPENDENCY_JOB_ENTRY::PARALLEL_DEPENDENCY_JOB_ENTRY(
    VOID
    ) :
    PARALLEL_DEPENDENCY_ENTRY()
{
    CurrentDependencyCount = 1; // the job entry is dependent on himself
}

// ~~

PARALLEL_DEPENDENCY_JOB_ENTRY::~PARALLEL_DEPENDENCY_JOB_ENTRY(
    VOID
    )
{
}

// .. OPERATIONS

VOID PARALLEL_DEPENDENCY_JOB_ENTRY::PropagateObjectIsReady(
    VOID
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_JOB_ENTRY::PropagateObjectIsNotReady(
    VOID
    )

{
    // :TODO: Assert if job is already schedule
}
