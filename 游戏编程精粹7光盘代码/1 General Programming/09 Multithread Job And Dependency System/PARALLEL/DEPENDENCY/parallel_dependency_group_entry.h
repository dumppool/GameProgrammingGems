#ifndef __PARALLEL_DEPENDENCY_GROUP_ENTRY__
    #define __PARALLEL_DEPENDENCY_GROUP_ENTRY__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_dependency_entry.h"
    #include "parallel_event.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_DEPENDENCY_GROUP_ENTRY :
        public PARALLEL_DEPENDENCY_ENTRY
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_GROUP_ENTRY(
            void
            );

        // ~~

        virtual ~PARALLEL_DEPENDENCY_GROUP_ENTRY(
            void
            );

        // .. OPERATORS

        // .. ACCESSORS

        // .. INQUIRIES

        // .. OPERATIONS

        virtual void PropagateObjectIsReady(
            void
            );

        // ~~

        virtual void PropagateObjectIsNotReady(
            void
            );

        // ~~

        void Wait(
            void
            )
        {
            Event.Wait();
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_GROUP_ENTRY(
            const PARALLEL_DEPENDENCY_GROUP_ENTRY & other
            );

        // .. OPERATORS

        PARALLEL_DEPENDENCY_GROUP_ENTRY & operator=(
            const PARALLEL_DEPENDENCY_GROUP_ENTRY & other
            );

        // .. ATTRIBUTES

        PARALLEL_EVENT
            Event;

    };
#endif
