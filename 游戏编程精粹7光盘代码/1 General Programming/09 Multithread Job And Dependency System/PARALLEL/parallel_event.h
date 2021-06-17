#ifndef __PARALLEL_EVENT__
    #define __PARALLEL_EVENT__

    // -- LOCAL

    // .. REFERENCES

	#include <Windows.h>
    #include "parallel.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_EVENT
    {
        // -- PUBLIC
        
    public :

        // .. CONSTRUCTORS

        PARALLEL_EVENT(
            void
            ) :
            Handle( NULL )
        {
        }

        // ~~

        PARALLEL_EVENT(
            const char * name
            );

        // ~~

        PARALLEL_EVENT(
            const char * name,
            const bool it_is_signaled,
            const bool it_is_manual_reset = false
            );

        // ~~

        ~PARALLEL_EVENT(
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

        void Create(
            const char * name,
            const bool it_is_signaled,
            const bool it_is_manual_reset = false
            );

        // ~~

        void SetExisting(
            const char * name
            );

        // ~~

        void Wait(
            void
            );

        // ~~

        bool Wait(
            const float duration
            );

        // ~~

        void Pulse(
            void
            );
        
        // ~~

        void Signal(
            void
            );

        // ~~

        void Reset(
            void
            );

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_EVENT::PARALLEL_EVENT(
            const PARALLEL_EVENT & other
            );

        // .. OPERATORS

        PARALLEL_EVENT & operator=(
            const PARALLEL_EVENT & other
            );

        // .. OPERATIONS

        // .. ATTRIBUTES
        
        PARALLEL_HANDLE
            Handle;
    };

    // .. IMPLEMENTATION

    #if PLATFORM_Pc
        #include "parallel_event_pc_members.hpp"
    #endif

#endif
