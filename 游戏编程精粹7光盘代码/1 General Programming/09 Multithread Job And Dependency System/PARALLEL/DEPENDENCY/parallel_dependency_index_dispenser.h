#ifndef __PARALLEL_DEPENDENCY_INDEX_DISPENSER__
    #define __PARALLEL_DEPENDENCY_INDEX_DISPENSER__

    // -- LOCAL

    // .. REFERENCES

    #include "counted_object.h"
    #include "primitive_index_dispenser.h"
    #include "parallel_lockable_object_of_.h"

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_DEPENDENCY_INDEX_DISPENSER :
        public COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_INDEX_DISPENSER(
            VOID
            );

        // ~~

        virtual ~PARALLEL_DEPENDENCY_INDEX_DISPENSER(
            VOID
            );

        // .. OPERATORS

        PARALLEL_LOCKABLE_OBJECT_OF_<PRIMITIVE_INDEX_DISPENSER> & operator ->(
            VOID
            )
        {
            return IndexDispenser;
        }

        static PARALLEL_DEPENDENCY_INDEX_DISPENSER & GetInstance(
            void
            )
        {
            if( Instance == 0 )
            {
                Instance = new PARALLEL_DEPENDENCY_INDEX_DISPENSER;
                Instance->IndexDispenser->SetIndexCount( 655536 );
            }

            return *Instance;
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_INDEX_DISPENSER(
            const PARALLEL_DEPENDENCY_INDEX_DISPENSER & other
            );

        // .. OPERATORS

        PARALLEL_DEPENDENCY_INDEX_DISPENSER & operator=(
            const PARALLEL_DEPENDENCY_INDEX_DISPENSER & other
            );

        // .. ATTRIBUTES

        PARALLEL_LOCKABLE_OBJECT_OF_<PRIMITIVE_INDEX_DISPENSER>
            IndexDispenser;
        static PARALLEL_DEPENDENCY_INDEX_DISPENSER
            *Instance;
    };

    // .. FUNCTIONS

    #define PARALLEL_DEPENDENCY_INDEX_DISPENSER_GetFreeIndex \
        PARALLEL_DEPENDENCY_INDEX_DISPENSER::GetInstance()->GetFreeIndex

    // ~~

    #define PARALLEL_DEPENDENCY_INDEX_DISPENSER_FreeIndex \
        PARALLEL_DEPENDENCY_INDEX_DISPENSER::GetInstance()->FreeIndex


#endif
