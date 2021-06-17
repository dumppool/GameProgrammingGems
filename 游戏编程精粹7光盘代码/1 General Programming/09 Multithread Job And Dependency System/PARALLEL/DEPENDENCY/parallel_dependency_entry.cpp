#include "parallel_dependency_entry.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel_dependency_identifier.h"
#include "parallel_dependency_manager.h"
#include "parallel_section.h"

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_DEPENDENCY_ENTRY::PARALLEL_DEPENDENCY_ENTRY(
    void
    ) :
    COUNTED_OBJECT(),
    CurrentDependencyCount( 0 )
{
}

// ~~

PARALLEL_DEPENDENCY_ENTRY::~PARALLEL_DEPENDENCY_ENTRY(
    void
    )
{
}

// .. OPERATIONS

void PARALLEL_DEPENDENCY_ENTRY::AddDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        DynamicDependendObjectTable.push_back( identifier );

        if ( CurrentDependencyCount != 0 )
        {
            PARALLEL_DEPENDENCY_MANAGER::GetInstance().GetDependencyEntryAtIdentifier( identifier ).IncrementDependencyCount();
        }
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_ENTRY::AddStaticDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        StaticDependendObjectTable.push_back( identifier );

        if ( CurrentDependencyCount != 0 )
        {
            PARALLEL_DEPENDENCY_MANAGER::GetInstance().GetDependencyEntryAtIdentifier( identifier ).IncrementDependencyCount();
        }
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_ENTRY::DecrementDependencyCount(
    void
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        --CurrentDependencyCount;

        if ( CurrentDependencyCount == 0 )
        {
            PropagateObjectIsReady();
        }
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_ENTRY::IncrementDependencyCount(
    void
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        if ( CurrentDependencyCount == 0 )
        {
            PropagateObjectIsNotReady();
        }

        ++CurrentDependencyCount;
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_ENTRY::DependencyIsMet(
    void
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        std::list<PARALLEL_DEPENDENCY_IDENTIFIER>::iterator
            identifier_iterator;
        PARALLEL_DEPENDENCY_MANAGER
            &manager = PARALLEL_DEPENDENCY_MANAGER::GetInstance();

        for( identifier_iterator = DynamicDependendObjectTable.begin(); 
                identifier_iterator != DynamicDependendObjectTable.end();
                    ++identifier_iterator )
        {
            manager.GetDependencyEntryAtIdentifier( *identifier_iterator ).DecrementDependencyCount();
        }

        DynamicDependendObjectTable.clear();

        for( identifier_iterator = StaticDependendObjectTable.begin(); 
            identifier_iterator != StaticDependendObjectTable.end();
            ++identifier_iterator )
        {
            manager.GetDependencyEntryAtIdentifier( *identifier_iterator ).DecrementDependencyCount();
        }
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_ENTRY::DependencyIsNotMet(
    void
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        std::list<PARALLEL_DEPENDENCY_IDENTIFIER>::iterator
            identifier_iterator;
        PARALLEL_DEPENDENCY_MANAGER
            &manager = PARALLEL_DEPENDENCY_MANAGER::GetInstance();

        for( identifier_iterator = DynamicDependendObjectTable.begin(); 
                identifier_iterator != DynamicDependendObjectTable.end();
                    ++identifier_iterator )
        {
            manager.GetDependencyEntryAtIdentifier( *identifier_iterator ).IncrementDependencyCount();
        }

        for( identifier_iterator = StaticDependendObjectTable.begin(); 
                identifier_iterator != StaticDependendObjectTable.end();
                    ++identifier_iterator )
        {
            manager.GetDependencyEntryAtIdentifier( *identifier_iterator ).IncrementDependencyCount();
        }
    }
    PARALLEL_SECTION_End()
}
