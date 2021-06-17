#ifndef __PARALLEL_ATOMICITY__
    #define __PARALLEL_ATOMICITY__

    // -- LOCAL

    // .. REFERENCES

    #include <windows.h>
    #include "parallel_platform.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_ATOMICITY
    {
        // -- PUBLIC

    public :

        #if PARALLEL_PLATFORM_Singlethread
            #include "parallel_atomicity_stub_members.h"
        #endif

        // .. CONSTRUCTORS

        PARALLEL_ATOMICITY(
            void
            );

        // ~~

        ~PARALLEL_ATOMICITY(
            void
            );

        // .. OPERATIONS

        void Begin(
            void
            );

        // ~~

        void End(
            void
            );

        // ~~

        bool TryBegin(
            void
            );

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_ATOMICITY(
            const PARALLEL_ATOMICITY & other
            );

        // .. OPERATORS

        PARALLEL_ATOMICITY & operator=(
            const PARALLEL_ATOMICITY & other
            );

        // .. ATTRIBUTES

        CRITICAL_SECTION
            CriticalSection;
        int
            EnteredCount;
    };

    // .. IMPLEMENTATION

    #if PARALLEL_PLATFORM_Singlethread
        #include "parallel_atomicity_stub_members.hpp"
    #elif PLATFORM_Pc
        #include "parallel_atomicity_pc_members.hpp"
    #endif
#endif
