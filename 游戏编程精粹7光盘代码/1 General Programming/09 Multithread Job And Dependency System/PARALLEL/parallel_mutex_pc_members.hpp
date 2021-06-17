// -- GLOBAL

inline PARALLEL_MUTEX::PARALLEL_MUTEX(
    const char * name
    )
{
    Handle = CreateMutex( NULL, FALSE, name );
    assert( Handle != NULL );
}

// ~~

inline PARALLEL_MUTEX::~PARALLEL_MUTEX(
    void
    )
{
    BOOL
        result;

    result = CloseHandle( Handle );
    assert( result != 0 );
}

// .. INQUIRIES

inline bool PARALLEL_MUTEX::IsValid(
    void
    )
{
    return Handle != NULL;
}

// .. OPERATIONS

inline void PARALLEL_MUTEX::Lock(
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

inline bool PARALLEL_MUTEX::TryLock(
    const float duration
    )
{
    unsigned int
        return_code;

    assert( Handle != NULL );

    return_code = WaitForSingleObject( Handle, DWORD( duration * 1000.0 ) );
    assert( return_code != WAIT_FAILED );

    return return_code == WAIT_OBJECT_0;
}

// ~~

inline void PARALLEL_MUTEX::Unlock(
    void
    )
{
	BOOL
        result;

    assert( Handle != NULL );

    result = ReleaseMutex( Handle );
    assert( result != 0 );
}
