// -- GLOBAL

inline PARALLEL_EVENT::PARALLEL_EVENT(
    const char * name
    )
{
    Handle = CreateEvent( NULL, true, false, name );
    assert( Handle != NULL );
}

// ~~

inline PARALLEL_EVENT::PARALLEL_EVENT(
    const char * name,
    const bool it_is_signaled,
    const bool it_is_manual_reset
    )
{
    Handle = CreateEvent( NULL, it_is_manual_reset, it_is_signaled, name );
    assert( Handle != NULL );
}

// ~~

inline PARALLEL_EVENT::~PARALLEL_EVENT(
    void
    )
{
    BOOL
        result;

    if ( Handle )
    {
        result = ::CloseHandle( Handle );
        assert( result != 0 );
    }
}

// .. INQUIRIES

inline bool PARALLEL_EVENT::IsValid(
    void
    )
{
    return Handle != NULL;
}

// .. OPERATIONS

inline void PARALLEL_EVENT::Create(
    const char * name,
    const bool it_is_signaled,
    const bool it_is_manual_reset
    )
{
    assert( Handle == NULL );
    Handle = ::CreateEvent( NULL, it_is_manual_reset, it_is_signaled, name );
}

// ~~

inline void PARALLEL_EVENT::SetExisting(
    const char * name
    )
{
    assert( Handle == NULL );

    Handle = ::OpenEvent( EVENT_ALL_ACCESS, false, name );
    assert( Handle != 0 );
}

// ~~

inline void PARALLEL_EVENT::Wait(
    void
    )
{
    UINT32
        return_code;
        
    assert( Handle != NULL );

    return_code = WaitForSingleObject( Handle, INFINITE );
    assert( return_code == WAIT_OBJECT_0 );
}

// ~~

inline bool PARALLEL_EVENT::Wait(
    const float duration
    )
{
    DWORD
        return_code;

    assert( Handle != NULL );

    return_code = WaitForSingleObject( Handle, (DWORD)( duration * 1000.0f ) );
    assert( return_code != WAIT_FAILED );

    return return_code == WAIT_OBJECT_0;
}

// ~~

inline void PARALLEL_EVENT::Signal(
    void
    )
{
    BOOL
        result;

    assert( Handle != NULL );

    result = ::SetEvent ( Handle );
    assert( result != 0 );
}

// ~~

inline void PARALLEL_EVENT::Pulse(
    void
    )
{
    BOOL
        result;

    assert( Handle != NULL );

    result = ::PulseEvent ( Handle );
    assert( result != 0 );
}

// ~~

inline void PARALLEL_EVENT::Reset(
    void
    )
{
    BOOL
        result;

    assert( Handle != NULL );

    result = ::ResetEvent ( Handle );
    assert( result != 0 );
}