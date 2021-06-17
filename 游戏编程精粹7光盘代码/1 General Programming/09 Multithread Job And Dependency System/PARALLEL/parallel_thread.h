#ifndef __PARALLEL_THREAD__
    #define __PARALLEL_THREAD__

    // -- LOCAL

    // .. REFERENCES

    #include "parallel.h"
    #include "parallel_mutex.h"
    #include "parallel_thread_identifier.h"
    #include "parallel_thread_state.h"
    #include "counted_object.h"

    #if PLATFORM_Pc
        #include "parallel_thread_pc_reference.h"
    #endif

    // -- GLOBAL

    // .. TYPES

    class PARALLEL_THREAD : 
        public COUNTED_OBJECT
    {
        // -- PUBLIC

    public :

        #if PLATFORM_Stub
            #include "parallel_thread_stub_members.h"
        #elif PLATFORM_Pc
            #include "parallel_thread_pc_members.h"
        #endif

        // .. CONSTRUCTORS

        PARALLEL_THREAD(
            VOID
            ) :
            Handle( NULL ),
            Identifier( 0 ),
            State( PARALLEL_THREAD_STATE_Constructed )
        {
        }

        // ~~

        virtual ~PARALLEL_THREAD(
            VOID
            );

        // .. OPERATORS

        operator PARALLEL_HANDLE(
            VOID
            )
        {
            return Handle;
        }

        // .. INQUIRIES

        const PARALLEL_THREAD_IDENTIFIER & GetIdentifier(
            VOID
            ) const
        {
            return Identifier;
        }

        // ~~

        PARALLEL_THREAD_STATE GetState(
            VOID
            ) const
        {
            return State;
        }

        // ~~

        BOOL IsSuspended(
            VOID
            ) const
        {
            return State == PARALLEL_THREAD_STATE_Suspended;
        }

        // ~~

        BOOL IsRunning(
            VOID
            ) const
        {
            return State == PARALLEL_THREAD_STATE_Running;
        }

        // ~~

        BOOL IsFinished(
            VOID
            ) const
        {
            return State == PARALLEL_THREAD_STATE_Finished;
        }

        // .. OPERATIONS

        VOID Create(
            const int processor_index = 0
            );

        // ~~

        VOID Start(
            const int processor_index = 0
            );

        #if !RETAIL_Mode
            VOID SetDebugName(
                const CHAR * thread_name
                );
        #endif

        // ~~

        VOID Suspend(
            VOID
            );

        // ~~

        VOID Resume(
            VOID
            );

        // ~~

        VOID Stop(
            VOID
            );

        // ~~

        VOID StopAndWait(
            VOID
            );

        // ~~

        VOID Terminate(
            const UINT32 exit_code
            );

        // ~~

        BOOL Wait(
            const float time = 0.0f
            );

        // ~~

        VOID SetPriority(
            const int priority
            );

        // ~~

        int GetPriority(
            VOID
            );

        // -- PROTECTED

    protected :

        // .. OPERATIONS

        virtual int Execute(
            VOID
            )
        {
            assert( false );
            return 0;
        }

        // -- PRIVATE

    private :

        // .. CONSTRUCTORS

        PARALLEL_THREAD(
            const PARALLEL_THREAD & other
            );

        // .. OPERATORS

        PARALLEL_THREAD & operator=(
            const PARALLEL_THREAD & other
            );

        // .. OPERATIONS

        int Run(
            VOID
            );

        // .. ATTRIBUTES

        PARALLEL_HANDLE
            Handle;
        PARALLEL_THREAD_IDENTIFIER
            Identifier;
        volatile PARALLEL_THREAD_STATE
            State;
        PARALLEL_MUTEX
            Data;
    };
#endif
