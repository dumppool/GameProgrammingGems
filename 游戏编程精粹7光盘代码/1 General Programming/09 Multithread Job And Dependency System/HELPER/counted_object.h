#ifndef __COUNTED_OBJECT__
    #define __COUNTED_OBJECT__

    // -- LOCAL

    // .. REFERENCES

    #include <assert.h>
   
    // -- GLOBAL

    // .. CONSTANTS

    // .. TYPES

    class COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        // .. CONSTRUCTORS

        COUNTED_OBJECT(
            void
            ) :
            RefCount( 0 ),
            LinkCount( 0 )
        {
        }

        // ~~

        COUNTED_OBJECT(
            const COUNTED_OBJECT & other
            ) :
            RefCount( 0 ),
            LinkCount( 0 )
        {
        }

        // ~~

        virtual ~COUNTED_OBJECT(
            void
            )
        {
        }

        // .. OPERATORS

        COUNTED_OBJECT & operator=(
            const COUNTED_OBJECT & other
            )
        {
            return *this;
        }

        // ~~

        int GetRefCount() const
        {
            return RefCount;
        }

        // ~~

        int GetLinkCount() const
        {
            return LinkCount;
        }

        // ~~

        void AddRef(
            void
            )
        {
            ++RefCount;
        }

        // ~~

        void RemoveRef(
            void
            )
        {
            --RefCount;

            if( RefCount == 0 )
            {
                assert( LinkCount == 0 );
                delete this;
            }
        }

        // ~~

        void AddLink(
            void
            )
        {
            ++LinkCount;
        }

        // ~~

        void RemoveLink(
            void
            )
        {
            --LinkCount;
        }

        // -- PROTECTED

    protected :

        // .. ATTRIBUTES

        int
            RefCount,
            LinkCount;
    };
   
#endif
