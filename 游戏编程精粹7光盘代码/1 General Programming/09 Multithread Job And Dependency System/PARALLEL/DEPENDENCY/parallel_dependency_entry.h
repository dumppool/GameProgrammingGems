#ifndef __PARALLEL_DEPENDENCY_ENTRY__
    #define __PARALLEL_DEPENDENCY_ENTRY__

    // -- LOCAL

    // .. REFERENCES

    #include "counted_object.h"
    #include "counted_ref_to_.h"
    #include "parallel_atomicity.h"
    #include "parallel_dependency_identifier.h"
    #include <list>

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_DEPENDENCY_ENTRY :
        public COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_ENTRY(
            void
            );

        // ~~

        virtual ~PARALLEL_DEPENDENCY_ENTRY(
            void
            );

        // .. ACCESSORS

        void SetUniqueIndex(
            const int unique_index
            )
        {
            UniqueIndex = unique_index;
        }

        // ~~

        int GetUniqueIndex(
            void
            ) const
        {
            return UniqueIndex;
        }

        // .. INQUIRIES

        int GetDependencyCount(
            void
            ) const
        {
            return CurrentDependencyCount;
        }

        // ~~

        virtual bool IsDependencyMet(
            void
            ) const
        {
            return CurrentDependencyCount == 0;
        }

        // .. OPERATIONS

        void AddDependency(
            const PARALLEL_DEPENDENCY_IDENTIFIER & indentifier
            );

        // ~~

        void AddStaticDependency(
            const PARALLEL_DEPENDENCY_IDENTIFIER & indentifier
            );

        // ~~

        void DecrementDependencyCount(
            void
            );

        // ~~

        virtual void IncrementDependencyCount(
            void
            );

        // ~~

        virtual void PropagateObjectIsReady(
            void
            ) = 0;

        // ~~

        virtual void PropagateObjectIsNotReady(
            void
            ) = 0;

        // ~~

        void DependencyIsMet(
            void
            );

        // ~~

        void DependencyIsNotMet(
            void
            );

        // -- PRIVATE

    protected :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_ENTRY(
            const PARALLEL_DEPENDENCY_ENTRY & other
            );

        // .. OPERATORS

        PARALLEL_DEPENDENCY_ENTRY & operator=(
            const PARALLEL_DEPENDENCY_ENTRY & other
            );

        // .. ATTRIBUTES

        std::list<PARALLEL_DEPENDENCY_IDENTIFIER>
            DynamicDependendObjectTable,
            StaticDependendObjectTable;
        int
            CurrentDependencyCount;
        PARALLEL_ATOMICITY
            Atomicity;
        int
            UniqueIndex;
    };

    typedef COUNTED_REF_TO_<PARALLEL_DEPENDENCY_ENTRY> 
        PARALLEL_DEPENDENCY_ENTRY_REF;
#endif
