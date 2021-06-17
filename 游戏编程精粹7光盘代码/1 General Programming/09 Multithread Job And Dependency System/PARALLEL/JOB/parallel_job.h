#ifndef __PARALLEL_JOB__
    #define __PARALLEL_JOB__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_job_function.h"
    #include "parallel_job_handle.h"
    #include "parallel_job_priority.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_JOB
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_JOB(
            void
            );

        // ~~

        PARALLEL_JOB(
            const PARALLEL_JOB & other
            );

        // ~~

        virtual ~PARALLEL_JOB(
            void
            );

        // .. OPERATORS

        PARALLEL_JOB & operator=(
            const PARALLEL_JOB & other
            );

        // .. OPERATORS

        bool operator<(
            const PARALLEL_JOB & other
            ) const
        {
            return Priority < other.Priority;
        }

        // .. ACCESSORS

        void SetJobHandle(
            const PARALLEL_JOB_HANDLE job_handle
            )
        {
            JobHandle = job_handle;
        }

        // ~~

        PARALLEL_JOB_HANDLE GetJobHandle(
            void
            ) const
        {
            return JobHandle;
        }

        // ~~

        void SetPriority(
            const PARALLEL_JOB_PRIORITY & priority
            )
        {
            Priority = priority;
        }

        // ~~

        const PARALLEL_JOB_PRIORITY & GetPriority(
            void
            ) const
        {
            return Priority;
        }

        // ~~

        void SetFunction(
            const PARALLEL_JOB_FUNCTION & function
            )
        {
            Function = function;
        }

        // ~~

        const PARALLEL_JOB_FUNCTION & GetFunction(
            void
            ) const
        {
            return Function;
        }

        // ~~

        void SetContext(
            void * context
            )
        {
            Context = context;
        }

        // ~~

        void * GetContext(
            void
            ) const
        {
            return Context;
        }

        // .. OPERATIONS

        void Execute()
        {
            Function( Context );
        }

        // -- PRIVATE

    private :

        // .. ATTRIBUTES

        PARALLEL_JOB_HANDLE
            JobHandle;
        PARALLEL_JOB_PRIORITY
            Priority;
        PARALLEL_JOB_FUNCTION
            Function;
        void *
            Context;
    };
#endif
