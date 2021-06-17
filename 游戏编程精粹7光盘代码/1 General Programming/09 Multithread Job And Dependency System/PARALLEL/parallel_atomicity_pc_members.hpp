// .. CONSTRUCTORS

inline PARALLEL_ATOMICITY::PARALLEL_ATOMICITY(
    void
    ) :
    CriticalSection(),
    EnteredCount( 0 )
{
    InitializeCriticalSection( &CriticalSection );
}

// ~~

inline PARALLEL_ATOMICITY::~PARALLEL_ATOMICITY(
    void
    )
{
    DeleteCriticalSection( &CriticalSection );
}

// .. OPERATIONS

inline void PARALLEL_ATOMICITY::Begin(
    void
    )
{
    EnterCriticalSection( &CriticalSection );
    ++EnteredCount;
}

// ~~

inline void PARALLEL_ATOMICITY::End(
    void
    )
{
    assert( EnteredCount );
    --EnteredCount;
    LeaveCriticalSection( &CriticalSection );
}

// ~~

inline bool PARALLEL_ATOMICITY::TryBegin(
    void
    )
{
    /*if( TryEnterCriticalSection( &CriticalSection ) !=0 )
    {
        ++EnteredCount;
        return true;
    }
    else*/
    {
        return false;
    }
}