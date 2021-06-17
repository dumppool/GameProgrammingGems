#ifndef __PARALLEL_DEPENDENCY_MANAGER__
    #define __PARALLEL_DEPENDENCY_MANAGER__

    // -- LOCAL

    // .. REFERENCES

    #include "counted_object.h"
    #include "primitive_index_dispenser.h"
    #include "parallel_atomicity.h"
    #include "parallel_dependency_entry.h"
    #include "parallel_dependency_identifier.h"
    #include "parallel_event.h"
    #include "parallel_job_handle.h"
    #include <string>
    #include <vector>

    // -- GLOBAL

    // .. CONSTANTS

    #define PARALLEL_DEPENDENCY_MANAGER_MaximumParallelWait    16

    // .. TYPES

    class PARALLEL_DEPENDENCY_MANAGER :
        public COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_MANAGER(
            void
            );

        // ~~

        virtual ~PARALLEL_DEPENDENCY_MANAGER(
            void
            );

        // .. ACCESSORS

        PARALLEL_DEPENDENCY_ENTRY & GetDependencyEntryAtIdentifier(
            const PARALLEL_DEPENDENCY_IDENTIFIER & identifer
            )
        {
            assert( DependencyTable[ identifer.GetTableIndex() ]->GetUniqueIndex() == identifer.GetUniqueIndex() );
            return *DependencyTable[ identifer.GetTableIndex() ];
        }

        // .. INQUIRIES

        bool PARALLEL_DEPENDENCY_MANAGER::IsDependencyMet(
            const PARALLEL_JOB_HANDLE & identifier
            ) const;

        // ~~

        bool IsDependencyMet(
            const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
            ) const;

        // .. OPERATIONS

        PARALLEL_DEPENDENCY_IDENTIFIER CreateDependencyEntry(
            const char* name = ""
            );

        // ~~

        PARALLEL_DEPENDENCY_IDENTIFIER CreateJobDependencyEntry(
            void
            );

        // ~~

        void AddDependency(
            const PARALLEL_JOB_HANDLE & job_handle,
            const PARALLEL_JOB_HANDLE & dependent_job_handle
            );

        // ~~

        void AddDependency(
            const PARALLEL_JOB_HANDLE & job_handle,
            const PARALLEL_DEPENDENCY_IDENTIFIER & index
            );

        // ~~

        void AddDependency(
            const PARALLEL_JOB_HANDLE & job_handle,
            const char* entry_name
            );

        // ~~

        void AddDependency(
            const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_index,
            const PARALLEL_DEPENDENCY_IDENTIFIER & index
            );

        // ~~

        void AddStaticDependency(
            const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_index,
            const PARALLEL_DEPENDENCY_IDENTIFIER & index
            );

        // ~~

        void AddStaticDependency(
            const char* dependency_name,
            const PARALLEL_DEPENDENCY_IDENTIFIER & index
            );

        // ~~

        void SetJobIsFinished(
            const PARALLEL_JOB_HANDLE & job_handle
            );

        // ~~

        void WaitForDependency(
            const PARALLEL_DEPENDENCY_IDENTIFIER & identifier
            );

        static PARALLEL_DEPENDENCY_MANAGER & GetInstance(
            void
            )
        {
            if( Instance == 0 )
            {
                Instance = new PARALLEL_DEPENDENCY_MANAGER();
            }

            return *Instance;
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_MANAGER(
            const PARALLEL_DEPENDENCY_MANAGER & other
            );

        // .. OPERATORS

        PARALLEL_DEPENDENCY_MANAGER & operator=(
            const PARALLEL_DEPENDENCY_MANAGER & other
            );

        // .. ATTRIBUTES

        std::vector< PARALLEL_DEPENDENCY_ENTRY_REF >
            DependencyTable;
        std::vector<std::string>
            DependencyNameTable;
        PARALLEL_ATOMICITY
            Atomicity;
        int
            NextDependencyIndex;

        static PARALLEL_DEPENDENCY_MANAGER
            * Instance;
    };

    // ~~ FUNCTIONS

    #define PARALLEL_DEPENDENCY_MANAGER_CreateDependencyEntry \
        PARALLEL_DEPENDENCY_MANAGER::GetInstance().CreateDependencyEntry

    // ~~

    #define PARALLEL_DEPENDENCY_MANAGER_AddDependency \
        PARALLEL_DEPENDENCY_MANAGER::GetInstance().AddDependency

    // ~~

    #define PARALLEL_DEPENDENCY_MANAGER_AddStaticDependency \
        PARALLEL_DEPENDENCY_MANAGER::GetInstance().AddStaticDependency

    // ~~

    #define PARALLEL_DEPENDENCY_MANAGER_WaitForDependency \
        PARALLEL_DEPENDENCY_MANAGER::GetInstance().WaitForDependency

#endif
