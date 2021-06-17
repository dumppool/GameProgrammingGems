#ifndef __PARALLEL_SECTION__
    #define __PARALLEL_SECTION__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel_atomicity.h"
    #include "parallel_platform.h"

    // -- GLOBAL

    #if PARALLEL_PLATFORM_Multithread

        // .. TYPES

        class PARALLEL_SECTION
        {
            // -- PUBLIC

        public :

            // .. CONSTRUCTORS

            PARALLEL_SECTION(
                PARALLEL_ATOMICITY & atomicity
                ) :
                Atomicity( atomicity )
            {
                Atomicity.Begin();
            }

            // ~~

            ~PARALLEL_SECTION(
                VOID
                )
            {
                Atomicity.End();
            }

            // -- PRIVATE

        private :

            // .. CONSTRUCTORS

            PARALLEL_SECTION(
                VOID
                );

            // ~~

            PARALLEL_SECTION(
                const PARALLEL_SECTION & other
                );

            // .. OPERATORS

            PARALLEL_SECTION & operator=(
                const PARALLEL_SECTION & other
                );

            // .. ATTRIBUTES

            PARALLEL_ATOMICITY
                & Atomicity;
        };

        // -- GLOBAL

        // .. FUNCTIONS

        #define PARALLEL_SECTION_Begin( atomicity ) \
            { PARALLEL_SECTION section( atomicity );

            // ~~

            #define PARALLEL_SECTION_End() \
            }
    #else
        #define PARALLEL_SECTION_Begin( atomicity ) \
            {
            // ~~

            #define PARALLEL_SECTION_End() \
            }
    #endif
#endif
