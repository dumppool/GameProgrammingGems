#ifndef __PARALLEL_DEPENDENCY_JOB_ENTRY__
    #define __PARALLEL_DEPENDENCY_JOB_ENTRY__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_dependency_entry.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_DEPENDENCY_JOB_ENTRY :
        public PARALLEL_DEPENDENCY_ENTRY
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_JOB_ENTRY(
            void
            );

        // ~~

        virtual ~PARALLEL_DEPENDENCY_JOB_ENTRY(
            void
            );

        // .. INQUIRIES


        bool IsDependencyMet(
            void
            ) const
        {
            return CurrentDependencyCount == 1;
        }

        // .. OPERATIONS

        virtual void PropagateObjectIsReady(
            void
            );

        // ~~

        virtual void PropagateObjectIsNotReady(
            void
            );

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_JOB_ENTRY(
            const PARALLEL_DEPENDENCY_JOB_ENTRY & other
            );

        // .. OPERATORS

        PARALLEL_DEPENDENCY_JOB_ENTRY & operator=(
            const PARALLEL_DEPENDENCY_JOB_ENTRY & other
            );
    };
#endif
