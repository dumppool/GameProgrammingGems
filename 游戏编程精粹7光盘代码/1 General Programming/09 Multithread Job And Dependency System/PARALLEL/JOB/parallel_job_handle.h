#ifndef __PARALLEL_JOB_HANDLE__
    #define __PARALLEL_JOB_HANDLE__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_dependency_identifier.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_JOB_HANDLE
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_JOB_HANDLE(
            void
            ) :
            Index( -1 ),
            DependencyIdentifier()
        {
        }

        // ~~

        PARALLEL_JOB_HANDLE(
            const int index,
            const PARALLEL_DEPENDENCY_IDENTIFIER & depedency_identifier
            ) :
            Index( index ),
            DependencyIdentifier( depedency_identifier )
        {
        }

        // ~~

        PARALLEL_JOB_HANDLE(
            const PARALLEL_JOB_HANDLE & other
            ) :
            Index( other.Index ),
            DependencyIdentifier( other.DependencyIdentifier )
        {

        }

        // ~~

        ~PARALLEL_JOB_HANDLE(
            void
            )
        {
        }

        // .. OPERATORS

        PARALLEL_JOB_HANDLE & operator=(
            const PARALLEL_JOB_HANDLE & other
            )
        {
            Index = other.Index;
            DependencyIdentifier = other.DependencyIdentifier;

            return *this;
        }

        // ~~

        bool operator ==( 
            const PARALLEL_JOB_HANDLE & other
            ) const
        {
            return Index == other.Index
                    && DependencyIdentifier == other.DependencyIdentifier;
        }

        // .. ACCESSORS

        void SetIndex(
            const int index
            )
        {
            Index = index;
        }

        // ~~

        int GetIndex(
            void
            ) const
        {
            return Index;
        }

        // ~~

        void SetDependencyIdentifier(
            const PARALLEL_DEPENDENCY_IDENTIFIER & dependency_identifier
            )
        {
            DependencyIdentifier = dependency_identifier;
        }

        // ~~

        const PARALLEL_DEPENDENCY_IDENTIFIER & GetDependencyIdentifier(
            void
            ) const
        {
            return DependencyIdentifier;
        }

        // -- PRIVATE

    private :

        // .. ATTRIBUTES

        int
            Index;
        PARALLEL_DEPENDENCY_IDENTIFIER
            DependencyIdentifier;
    };
#endif
