// -- GLOBAL

inline PARALLEL_MUTEX::PARALLEL_MUTEX(
    const char * name
    )
{
}

// ~~

inline PARALLEL_MUTEX::~PARALLEL_MUTEX(
    void
    )
{
}

// .. INQUIRIES

inline bool PARALLEL_MUTEX::IsValid(
    void
    )
{
    return false;
}

// .. OPERATIONS

inline void PARALLEL_MUTEX::Lock(
    void
    )
{    
}

// ~~

inline bool PARALLEL_MUTEX::TryLock(
    const float duration
    )
{
    return false;
}

// ~~

inline void PARALLEL_MUTEX::Unlock(
    void
    )
{
}
