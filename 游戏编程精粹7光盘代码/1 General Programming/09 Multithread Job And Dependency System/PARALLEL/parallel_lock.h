#ifndef __PARALLEL_LOCK__
    #define __PARALLEL_LOCK__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_mutex.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_LOCK
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_LOCK(
            const PARALLEL_MUTEX & mutex
            ) :
			Object( const_cast< PARALLEL_MUTEX& >( mutex ) )
        {
            Object.Lock();
        }

        // ~~

        ~PARALLEL_LOCK(
            VOID
            )
        {
            Object.Unlock();
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_LOCK(
            VOID
            );

        // ~~

        PARALLEL_LOCK(
            const PARALLEL_LOCK & other
            );

        // .. OPERATORS

        PARALLEL_LOCK & operator=(
            const PARALLEL_LOCK & other
            );

        // .. ATTRIBUTES

        PARALLEL_MUTEX
            & Object;
    };
#endif
