#ifndef __PARALLEL_LOCKABLE_OBJECT_OF___
    #define __PARALLEL_LOCKABLE_OBJECT_OF___

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_atomicity.h"

    // -- GLOBAL

    // .. TYPES

    template< typename _TYPE_ >
    class PARALLEL_LOCKABLE_OBJECT_OF_
    {
        // -- PUBLIC

    public :

        // .. TYPE

        template< typename _LOCKED_TYPE_ >
        class LOCKING_PROXY_OF_
        {
            // -- PUBLIC

        public :

            // .. CONSTRUCTORS

            LOCKING_PROXY_OF_(
                _LOCKED_TYPE_ * object,
                PARALLEL_ATOMICITY * atomicity
                ) :
                Object( object ),
                Atomicity( atomicity )
            {
                Atomicity->Begin();
            }

            // ~~

            ~LOCKING_PROXY_OF_(
                VOID
                )
            {
                Atomicity->End();
            }

            // .. OPERATORS

            _LOCKED_TYPE_ * operator->()
            {
                return Object;
            }

            // -- PRIVATE

        private :

            // .. ATTRIBUTES

            PARALLEL_ATOMICITY
                * Atomicity;
            _LOCKED_TYPE_
                * Object;
        };

        // .. CONSTRUCTORS

        PARALLEL_LOCKABLE_OBJECT_OF_(
            VOID
            )
        {
        }

        // ~~

        ~PARALLEL_LOCKABLE_OBJECT_OF_(
            VOID
            )
        {
        }

        // .. OPERATORS

        LOCKING_PROXY_OF_<_TYPE_> operator->()
        {
            return LOCKING_PROXY_OF_<_TYPE_>( &Object, &Atomicity );
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_LOCKABLE_OBJECT_OF_(
            const PARALLEL_LOCKABLE_OBJECT_OF_ & other
            );

        // .. OPERATORS

        PARALLEL_LOCKABLE_OBJECT_OF_ & operator=(
            const PARALLEL_LOCKABLE_OBJECT_OF_ & other
            );

        // .. ATTRIBUTES

        _TYPE_
            Object;
        PARALLEL_ATOMICITY
            Atomicity;
    };
#endif
