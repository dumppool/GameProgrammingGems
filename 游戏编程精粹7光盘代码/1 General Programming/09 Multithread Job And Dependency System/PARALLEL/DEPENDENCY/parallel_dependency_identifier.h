#ifndef __PARALLEL_DEPENDENCY_IDENTIFIER__
    #define __PARALLEL_DEPENDENCY_IDENTIFIER__

    // -- LOCAL

    // .. REFERENCES

    // -- GLOBAL

    // .. TYPES

    struct PARALLEL_DEPENDENCY_IDENTIFIER
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        PARALLEL_DEPENDENCY_IDENTIFIER(
            void
            )
        {
            TableIndex = -1;
            UniqueIndex = -1;
        }

        // ~~

        PARALLEL_DEPENDENCY_IDENTIFIER(
            const int table_index,
            const int unique_index
            )
        {
            TableIndex = table_index;
            UniqueIndex = unique_index;
        }

        // .. OPERATORS

        bool operator ==(
            const PARALLEL_DEPENDENCY_IDENTIFIER & other 
            ) const
        {
            return TableIndex == other.TableIndex
                && UniqueIndex == other.UniqueIndex;
        }

        // .. ACCESSORS

        void SetTableIndex(
            const int index
            )
        {
            TableIndex = index;
        }

        // ~~

        int GetTableIndex(
            void
            ) const
        {
            return TableIndex;
        }

        // ~~

        void SetUniqueIndex(
            const int index
            )
        {
            UniqueIndex = index;
        }

        // ~~

        int GetUniqueIndex(
            void
            ) const
        {
            return UniqueIndex;
        }

        // -- PRIVATE

    private :

        // .. ATTRIBUTES

        int
            TableIndex,
            UniqueIndex;
    };
#endif
