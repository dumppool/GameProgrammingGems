#ifndef __PARALLEL_MUTEX__
    #define __PARALLEL_MUTEX__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel.h"
    #include "parallel_platform.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_MUTEX
    {
        // -- PUBLIC
        
    public :

        // .. CONSTRUCTORS

        PARALLEL_MUTEX(
            const char * name = NULL
            );

        // ~~

        virtual ~PARALLEL_MUTEX(
            void
            );

        // .. OPERATORS

        operator PARALLEL_HANDLE(
            void
            )
        {
            return Handle;
        }

        // .. INQUIRIES

        bool IsValid(
            void
            );

        // .. OPERATIONS

        void Lock(
            void
            );

        // ~~

        bool TryLock(
            const float duration = 0.0f
            );

        // ~~

        void Unlock(
            void
            );

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_MUTEX(
            const PARALLEL_MUTEX & other
            );

        // .. OPERATORS

        PARALLEL_MUTEX & operator=(
            const PARALLEL_MUTEX & other
            );

        // .. OPERATIONS

        // .. ATTRIBUTES
        
        PARALLEL_HANDLE
            Handle;
    };

    // .. IMPLEMENTATION

    #if PARALLEL_PLATFORM_Singlethread
        #include "parallel_mutex_stub_members.hpp"
    #else
        #include "parallel_mutex_pc_members.hpp"
    #endif

#endif
