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
#include <algorithm>

// -- PUBLIC

// .. CONSTRUCTORS

PARALLEL_DEPENDENCY_MANAGER::PARALLEL_DEPENDENCY_MANAGER(
    void
    ) :
    COUNTED_OBJECT(),
    NextDependencyIndex( 0 )
{
}

// ~~

PARALLEL_DEPENDENCY_MANAGER::~PARALLEL_DEPENDENCY_MANAGER(
    void
    )
{
}

// .. INQUIRIES

bool PARALLEL_DEPENDENCY_MANAGER::IsDependencyMet(
    const PARALLEL_JOB_HANDLE & identifier
    ) const
{
    return IsDependencyMet( identifier.GetDependencyIdentifier() );
}

// ~~

bool PARALLEL_DEPENDENCY_MANAGER::IsDependencyMet(
    const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
    ) const
{
    return ( DependencyTable[ identifier.GetTableIndex() ].IsNull() 
        || DependencyTable[ identifier.GetTableIndex() ]->GetUniqueIndex() != identifier.GetUniqueIndex() 
        || DependencyTable[ identifier.GetTableIndex() ]->IsDependencyMet()
        );
}

// .. OPERATIONS

PARALLEL_DEPENDENCY_IDENTIFIER PARALLEL_DEPENDENCY_MANAGER::CreateDependencyEntry(
    const char * name
    )
{
    int
        table_index;

    PARALLEL_SECTION_Begin( Atomicity )
    {
        table_index = PARALLEL_DEPENDENCY_INDEX_DISPENSER_GetFreeIndex();

        if ( table_index >= int( DependencyTable.size() ) )
        {
            DependencyTable.resize( table_index + 1 );
            DependencyNameTable.resize( table_index + 1 );
        }

        DependencyTable[ table_index ] = new PARALLEL_DEPENDENCY_GROUP_ENTRY;
        ++NextDependencyIndex;
        DependencyTable[ table_index ]->SetUniqueIndex( NextDependencyIndex );
        DependencyNameTable[ table_index ] = name;
    }
    PARALLEL_SECTION_End()

    return PARALLEL_DEPENDENCY_IDENTIFIER( table_index, NextDependencyIndex );
}

// ~~

PARALLEL_DEPENDENCY_IDENTIFIER PARALLEL_DEPENDENCY_MANAGER::CreateJobDependencyEntry(
    void
    )
{
    int
        table_index;

    PARALLEL_SECTION_Begin( Atomicity )
    {
        table_index = PARALLEL_DEPENDENCY_INDEX_DISPENSER_GetFreeIndex();

        if ( table_index >= int( DependencyTable.size() ) )
        {
            DependencyTable.resize( table_index + 1 );
            DependencyNameTable.resize( table_index + 1 );
        }

        DependencyTable[ table_index ] = new PARALLEL_DEPENDENCY_JOB_ENTRY;
        ++NextDependencyIndex;
        DependencyTable[ table_index ]->SetUniqueIndex( NextDependencyIndex );
        DependencyNameTable[ table_index ] = "job";
    }
    PARALLEL_SECTION_End()

    return PARALLEL_DEPENDENCY_IDENTIFIER( table_index, NextDependencyIndex );
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_JOB_HANDLE & job_handle,
    const PARALLEL_JOB_HANDLE & dependent_job_handle
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        if( !DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ].IsNull()
            && DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->GetUniqueIndex() 
                == job_handle.GetDependencyIdentifier().GetUniqueIndex() 
            )
        {
            DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->AddDependency( 
                dependent_job_handle.GetDependencyIdentifier()
                );
        }
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_index,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        if( dependency_index.GetUniqueIndex() == DependencyTable[ dependency_index.GetTableIndex() ]->GetUniqueIndex() )
			DependencyTable[ dependency_index.GetTableIndex() ]->AddDependency( index );
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::AddStaticDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_index,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
		if( dependency_index.GetUniqueIndex() == DependencyTable[ dependency_index.GetTableIndex() ]->GetUniqueIndex() )
			DependencyTable[ dependency_index.GetTableIndex() ]->AddStaticDependency( index );
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::AddStaticDependency(
    const char * dependency_name,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        int
            dependency_index;
        std::vector<std::string>::iterator
            iterator;

        iterator = std::find( DependencyNameTable.begin(), DependencyNameTable.end(), dependency_name );

        assert( iterator != DependencyNameTable.end() );

        dependency_index = iterator - DependencyNameTable.begin();

        DependencyTable[ dependency_index ]->AddStaticDependency(
            index
            );
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_JOB_HANDLE & job_handle,
    const PARALLEL_DEPENDENCY_IDENTIFIER & index
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {     
        if( DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->GetUniqueIndex() == job_handle.GetDependencyIdentifier().GetUniqueIndex() )
			DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->AddDependency(
				index
				);
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::AddDependency(
    const PARALLEL_JOB_HANDLE & job_handle,
    const char * entry_name
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        int
            dependency_index;
        std::vector<std::string>::iterator
            iterator;

        iterator = std::find( DependencyNameTable.begin(), DependencyNameTable.end(), entry_name );

        assert( iterator != DependencyNameTable.end() );

        dependency_index = iterator - DependencyNameTable.begin();

        assert( DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->GetUniqueIndex() 
            == job_handle.GetDependencyIdentifier().GetUniqueIndex() );

        DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->AddDependency(
            PARALLEL_DEPENDENCY_IDENTIFIER( dependency_index, DependencyTable[ dependency_index ]->GetUniqueIndex() )
            );
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::SetJobIsFinished(
    const PARALLEL_JOB_HANDLE & job_handle
    )
{
    PARALLEL_SECTION_Begin( Atomicity )
    {
        assert( DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->GetUniqueIndex() 
            == job_handle.GetDependencyIdentifier().GetUniqueIndex() );

        DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ]->DependencyIsMet();
        PARALLEL_DEPENDENCY_INDEX_DISPENSER_FreeIndex( job_handle.GetDependencyIdentifier().GetTableIndex() );
        DependencyTable[ job_handle.GetDependencyIdentifier().GetTableIndex() ].SetNull();
    }
    PARALLEL_SECTION_End()
}

// ~~

void PARALLEL_DEPENDENCY_MANAGER::WaitForDependency(
    const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
    )
{
    // Only works on group
    PARALLEL_DEPENDENCY_GROUP_ENTRY
        & entry = static_cast<PARALLEL_DEPENDENCY_GROUP_ENTRY&>( *DependencyTable[ identifier.GetTableIndex() ] );

    assert( dynamic_cast<PARALLEL_DEPENDENCY_GROUP_ENTRY*>( DependencyTable[ identifier.GetTableIndex() ].GetPointer() ) );
      
    entry.Wait();
}