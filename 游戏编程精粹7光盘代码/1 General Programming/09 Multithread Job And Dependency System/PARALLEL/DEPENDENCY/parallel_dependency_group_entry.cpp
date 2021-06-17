#include "parallel_dependency_group_entry.h"

// -- LOCAL

// .. REFERENCES

#include "parallel_section.h"

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_DEPENDENCY_GROUP_ENTRY::PARALLEL_DEPENDENCY_GROUP_ENTRY(
    VOID
    ) :
    PARALLEL_DEPENDENCY_ENTRY()
{
    Event.Create( 0, true , true );
}

// ~~

PARALLEL_DEPENDENCY_GROUP_ENTRY::~PARALLEL_DEPENDENCY_GROUP_ENTRY(
    VOID
    )
{
}

// .. OPERATIONS

VOID PARALLEL_DEPENDENCY_GROUP_ENTRY::PropagateObjectIsReady(
    VOID
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        DependencyIsMet();
        Event.Signal();
    }
    PARALLEL_SECTION_End()
}

// ~~

VOID PARALLEL_DEPENDENCY_GROUP_ENTRY::PropagateObjectIsNotReady(
    VOID
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        Event.Reset();
        DependencyIsNotMet();
    }
    PARALLEL_SECTION_End()
}