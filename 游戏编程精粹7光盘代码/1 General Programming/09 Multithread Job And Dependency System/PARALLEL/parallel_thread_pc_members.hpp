#include "parallel_lock.h"

// -- GLOBAL

// .. CONSTANTS

#define LOCAL_VisualStudioException 0x406D1388

// .. CONSTRUCTORS

PARALLEL_THREAD::~PARALLEL_THREAD(
    VOID
    )
{
    if ( Handle != NULL )
    {
        assert( WaitForSingleObject( Handle, 0 ) == WAIT_OBJECT_0 );
        CloseHandle( Handle );
    }
}

// .. OPERATIONS

VOID PARALLEL_THREAD::Create(
    const int processor_index
    )
{
    int
        result;
    PARALLEL_LOCK
        protect( Data );

    assert( Handle == NULL );

    State = PARALLEL_THREAD_STATE_Starting;
    result = _beginthreadex( NULL, 0, &Begin, this, CREATE_SUSPENDED, &Identifier );
    assert( result != 0 );
    Handle = (HANDLE)result;
    State = PARALLEL_THREAD_STATE_Suspended;
}

// ~~

VOID PARALLEL_THREAD::Start(
    const int processor_index
    )
{
    Create( processor_index );
    Resume();
}

// ~~

#if !RETAIL_Mode

    struct LOCAL_THREAD_INFO
    {
        DWORD dwType; // Must be 0x1000.
        LPCSTR szName; // Pointer to name (in user addr space).
        DWORD dwHandleID; // Handle ID (-1=caller thread).
        DWORD dwFlags; // Reserved for future use, must be zero.
    };

    // :TRICKY: Set the thread's name in the debugger

    VOID PARALLEL_THREAD::SetDebugName(
        const CHAR * thread_name
        )
    {
        LOCAL_THREAD_INFO
            info;

        assert( Handle != NULL );

        info.dwType = 0x1000;
        info.szName = thread_name;
        info.dwHandleID = Identifier;
        info.dwFlags = 0;

        __try
        {
            RaiseException( LOCAL_VisualStudioException, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info );
        }
        __except(EXCEPTION_CONTINUE_EXECUTION)
        {
        }
    }
#endif

// ~~

VOID PARALLEL_THREAD::Suspend(
    VOID
    )
{
    DWORD
        result;
    PARALLEL_LOCK
        protect( Data );

    assert( Handle != NULL );
    assert( State == PARALLEL_THREAD_STATE_Running );

    State = PARALLEL_THREAD_STATE_Suspended;
    
    result = ::SuspendThread( Handle );
    assert( result != ( DWORD )-1 );
}

// ~~

VOID PARALLEL_THREAD::Resume(
    VOID
    )
{
    DWORD
        result;
    PARALLEL_LOCK
        protect( Data );

    assert( Handle != NULL );
    assert( ( State == PARALLEL_THREAD_STATE_Suspended ) || ( State == PARALLEL_THREAD_STATE_Starting ) );
   
    result = ::ResumeThread( Handle );
    assert( result != ( DWORD )-1 );
    State = PARALLEL_THREAD_STATE_Running;
}

// ~~

VOID PARALLEL_THREAD::Stop(
    VOID
    )
{
    PARALLEL_LOCK
        protect( Data );

    assert( Handle != NULL );

    if ( State == PARALLEL_THREAD_STATE_Suspended )
    {
        Resume();
    }

    assert( State == PARALLEL_THREAD_STATE_Running || State == PARALLEL_THREAD_STATE_Finished );
    State = PARALLEL_THREAD_STATE_Stopping;
}

// ~~

VOID PARALLEL_THREAD::StopAndWait(
    VOID
    )
{
    #if DEBUG_Mode
        HRESULT
            result;
    #endif

    Stop();
    #if !DEBUG_Mode
        WaitForSingleObject( Handle, INFINITE );
    #else
        while( ( result = WaitForSingleObject( Handle, 5000 ) ) == WAIT_TIMEOUT )
        {
            PRIMITIVE_Log3( "Thread %x is currently waiting for thread %x\n", GetCurrentThreadId(), Handle );
        }

        assert( result == WAIT_OBJECT_0 );
    #endif

}

// ~~

BOOL PARALLEL_THREAD::Wait(
    const float time
    )
{
    DWORD
        wait_millisecond;

    wait_millisecond = DWORD( time * 1000.0 );

    if( wait_millisecond > 0 )
    {
       return WaitForSingleObject( Handle, wait_millisecond ) == WAIT_OBJECT_0;
    }
    else
    {
        return WaitForSingleObject( Handle, INFINITE ) == WAIT_OBJECT_0;
    }
}

// ~~

VOID PARALLEL_THREAD::Terminate(
    const UINT32 exit_code
    )
{
    BOOL
        result;
    PARALLEL_LOCK
        protect( Data );

    assert( State != PARALLEL_THREAD_STATE_Constructed );

    if( Handle != NULL )
    {
        result = TerminateThread( Handle, exit_code );
        assert( result != 0 );
    }
}

// ~~

VOID PARALLEL_THREAD::SetPriority(
    const int priority
    )
{
    assert( Handle );

    SetThreadPriority( Handle, priority );
}

// ~~

int PARALLEL_THREAD::GetPriority(
    VOID
    )
{
    assert( Handle );

    return GetThreadPriority( Handle );
}

// -- PRIVATE

// .. OPERATIONS

int PARALLEL_THREAD::Run(
    VOID
    )
{
    INT
        result;

    assert( Handle != NULL );

    //REALITY2_Log( 0, "Begin thread %d\n", GetIdentifier() );

    result = Execute();
    PARALLEL_Code( Data, State = PARALLEL_THREAD_STATE_Finished );

    //REALITY2_Log( 0, "End thread %d\n", GetIdentifier() );

    return result;
}

// .. FUNCTIONS

unsigned int __stdcall PARALLEL_THREAD::Begin(
    VOID * thread
    )
{
    assert( thread != NULL );

    return static_cast<PARALLEL_THREAD *>( thread )->Run();
}
