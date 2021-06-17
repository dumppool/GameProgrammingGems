#ifndef __COUNTED_REF_TO__
    #define __COUNTED_REF_TO__

    // -- GLOBAL

    // .. TYPES

    template <class _OBJECT_>
    class COUNTED_REF_TO_
    {
        // -- PUBLIC

    public :

        typedef _OBJECT_ OBJECT;

        // .. CONSTRUCTORS

        COUNTED_REF_TO_(
            void
            ):
            Object( 0 )
        {
        }

        // ~~

        COUNTED_REF_TO_(
            const COUNTED_REF_TO_ & other
            ):
            Object( 0 )
        {
            SetObject( other.Object );
        }

        // ~~

        COUNTED_REF_TO_(
            const _OBJECT_ * object
            ):
            Object( NULL )
        {
            SetObject( object );
        }

        // ~~

        ~COUNTED_REF_TO_(
            void
            )
        {
            SetNull();
        }

        // .. OPERATORS

        COUNTED_REF_TO_ & operator=(
            const COUNTED_REF_TO_ & other
            )
        {
            SetObject( other.Object );

            return *this;
        }

        // ~~

        COUNTED_REF_TO_ & operator=(
            const _OBJECT_ * object
            )
        {
            SetObject( object );

            return *this;
        }

        // ~~

        operator _OBJECT_ *( 
            void
            ) const
        {
            return Object;
        }

        // ~~

        _OBJECT_ & operator*( 
            void
            ) const
        {
            return *Object;
        }

        // ~~

        _OBJECT_ * operator->( 
            void
            ) const
        {
            return Object;
        }

        // ~~

        operator _OBJECT_ &(
            void
            )
        {
            return Object;
        }

        // ~~

        operator const _OBJECT_ &(
            void
            ) const
        {
            return Object;
        }

        const _OBJECT_* GetPointer(
            void
            ) const 
        {
            return Object;
        }

        _OBJECT_* GetPointer(
            void
            ) 
        {
            return Object;
        }

        bool IsNull(
            void
            ) const
        {
            return Object == 0;
        };

        // .. OPERATIONS

        void SetNull(
            void
            )
        {
            if ( Object != 0 )
            {
                Object->RemoveRef();
                Object = 0;
            }
        }

        // ~~

        void SetObject(
            const _OBJECT_ * object
            )
        {
            if ( Object != object )
            {
                if ( object != 0 )
                {
                    const_cast<_OBJECT_*>( object )->AddRef();
                }

                // inlined SetNull();
                if ( Object != 0 )
                {
                    Object->RemoveRef();
                }

                Object = const_cast<_OBJECT_*>( object );
            }
        }


    private:

        _OBJECT_
            * Object;
    };
             
#endif
