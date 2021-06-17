#ifndef __PRIMITIVE_INDEX_DISPENSER__
    #define __PRIMITIVE_INDEX_DISPENSER__

    // -- LOCAL

    // .. REFERENCES

    #include "counted_object.h"
    #include <vector>

    // -- GLOBAL

    // .. TYPES

    class PRIMITIVE_INDEX_DISPENSER :
        public COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PRIMITIVE_INDEX_DISPENSER(
            void
            ) :
            COUNTED_OBJECT(),
            IndexCount( 0 ),
            FirstFreeIndex( -1 ),
            NextFreeIndexTable()
        {
        }

        // ~~

        PRIMITIVE_INDEX_DISPENSER(
            const int index_count
            ) :
            COUNTED_OBJECT(),
            IndexCount( 0 ),
            FirstFreeIndex( -1 ),
            NextFreeIndexTable()
        {
            SetIndexCount( index_count );
        }

        // ~~

        virtual ~PRIMITIVE_INDEX_DISPENSER(
            void
            )
        {
        }

        // .. INQUIRIES

        bool HasFreeIndex(
            void
            ) const
        {
            return FirstFreeIndex != -1;
        }

        // .. OPERATIONS

        void FreeAllIndexes(
            void
            );

        // ~~

        void SetIndexCount(
            const int index_count
            );

        // ~~

        int GetFreeIndex(
            void
            );

        // ~~

        void FreeIndex(
            const int used_index
            );

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PRIMITIVE_INDEX_DISPENSER(
            const PRIMITIVE_INDEX_DISPENSER & other
            );

        // .. OPERATORS

        PRIMITIVE_INDEX_DISPENSER & operator=(
            const PRIMITIVE_INDEX_DISPENSER & other
            );

        // .. ATTRIBUTES

        int
            IndexCount,
            FirstFreeIndex;
        std::vector<int>
            NextFreeIndexTable;
    };
#endif
