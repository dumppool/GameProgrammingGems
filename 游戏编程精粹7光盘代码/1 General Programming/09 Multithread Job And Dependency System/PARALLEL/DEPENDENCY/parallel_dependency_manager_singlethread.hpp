#include "parallel_dependency_manager.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel_dependency_group_entry.h"
#include "parallel_dependency_identifier.h"
#include "parallel_dependency_index_dispenser.h"
#include "parallel_dependency_job_entry.h"
#include "parallel_job_handle.h"
#include "parallel_section.h"

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_DEPENDENCY_MANAGER::PARALLEL_DEPENDENCY_MANAGER(
    VOID
    ) :
    COUNTED_OBJECT()
{
}

// ~~

PARALLEL_DEPENDENCY_MANAGER::~PARALLEL_DEPENDENCY_MANAGER(
    VOID
    )
{
}

// .. INQUIRIES

bool PARALLEL_DEPENDENCY_MANAGER::IsDependencyMet(
     const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
     ) const
{
    return true;
}

// .. OPERATIONS

PARALLEL_DEPENDENCY_IDENTIFIER PARALLEL_DEPENDENCY_MANAGER::CreateDependencyEntry(
    const char * name
    )
{
    return PARALLEL_DEPENDENCY_IDENTIFIER();
}

// ~~

PARALLEL_DEPENDENCY_IDENTIFIER PARALLEL_DEPENDENCY_MANAGER::CreateJobDependencyEntry(
    VOID
    )
{
    return PARALLEL_DEPENDENCY_IDENTIFIER();
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::AddDependency(
   const PARALLEL_JOB_HANDLE & job_handle,
   const PARALLEL_JOB_HANDLE & dependent_job_handle
   )
{

}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_index,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::AddStaticDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_index,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::AddStaticDependency(
    const char * dependency_name,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_JOB_HANDLE & job_handle,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_JOB_HANDLE & job_handle,
    const char * entry_name
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::SetJobIsFinished(
    const PARALLEL_JOB_HANDLE & job_handle
    )
{
}

// ~~

VOID PARALLEL_DEPENDENCY_MANAGER::WaitForDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
    )
{
}
