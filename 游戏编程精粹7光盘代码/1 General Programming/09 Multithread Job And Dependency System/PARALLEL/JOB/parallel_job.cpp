#include "parallel_job.h"

// -- LOCAL

// .. REFERENCES

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_JOB::PARALLEL_JOB(
    void
    ):
    JobHandle(),
	Priority( PARALLEL_JOB_PRIORITY_Default ),
    Function( 0 ),
    Context( 0 )
{
}

// ~~

PARALLEL_JOB::PARALLEL_JOB(
    const PARALLEL_JOB & other
    ):
    JobHandle( other.JobHandle ),
	Priority( other.Priority ),
    Function( other.Function ),
    Context( other.Context )
{
}

// ~~

PARALLEL_JOB::~PARALLEL_JOB(
    void
    )
{
}

// .. OPERATORS

PARALLEL_JOB & PARALLEL_JOB::operator=(
    const PARALLEL_JOB & other
    )
{
    JobHandle = other.JobHandle;
	Priority = other.Priority;
    Function = other.Function;
    Context = other.Context;

    return *this;
}
