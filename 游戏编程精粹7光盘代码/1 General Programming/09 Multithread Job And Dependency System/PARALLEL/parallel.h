#ifndef __PARALLEL__
    #define __PARALLEL__
    
    // -- LOCAL
    
    // .. REFERENCES
    
	#if PLATFORM_Pc
		#include <windows.h>
		#include <intrin.h>
	#endif
    #include "parallel_handle.h"
    #include "parallel_thread_identifier.h"

    // -- GLOBAL

    // .. TYPES
    
    // .. FUNCTIONS
   
    inline VOID PARALLEL_SleepMillisecondCount(
        DWORD millisecond_count
        )
    {
        Sleep( millisecond_count );
    }
    
    // ~~

    inline VOID PARALLEL_SleepSecondCount(
        float second_count
        )
    {
        PARALLEL_SleepMillisecondCount( DWORD( second_count * 1000.0 ) );
    }

    // ~~

    #define PARALLEL_Code( _mutex_, _code_ ) \
        { PARALLEL_LOCK protect( _mutex_ ); _code_; }

    // ~~

    #define PARALLEL_ProtectBegin( _mutex_ ) \
        { PARALLEL_LOCK protect( _mutex_ );

    // ~~

    #define PARALLEL_ProtectEnd() \
        }

    #if PARALLEL_PLATFORM_Singlethread

        #define PARALLEL_GetCurrentThread  0 
        #define PARALLEL_GetCurrentThreadIdentifier 0
        #define PARALLEL_GetProcessorCount() 1

        #define PARALLEL_Decrement( _data_ ) ( --( *_data_ ) )
        #define PARALLEL_Increment( _data_ ) ( ++( *_data_ ) )
        #define PARALLEL_Exchange( _data_, _value_ ) ( ( *_data_ ) == _value_ )
        #define PARALLEL_And( _data_, _mask_ ) ( (*data) & _mask_ )
        #define PARALLEL_Or( _data_, _mask_ ) ( (*data) | _mask_ )
        #define PARALLEL_Xor( _data_, _mask_ ) ( (*data) ^ _mask_ )
        #define PARALLEL_Infinite INFINITE

    #elif PLATFORM_Pc
        // the data must be aligned on a 32-bit boundary
        #define PARALLEL_GetCurrentThread ::GetCurrentThread
        #define PARALLEL_GetCurrentThreadIdentifier ::GetCurrentThreadId
        #define PARALLEL_GetProcessorCount() 2

        #pragma intrinsic(_InterlockedDecrement)
        #pragma intrinsic(_InterlockedIncrement)
        #pragma intrinsic(_InterlockedExchange)
        #pragma intrinsic(_InterlockedAnd)
        #pragma intrinsic(_InterlockedOr)
        #pragma intrinsic(_InterlockedXor)

        #define PARALLEL_Decrement ::_InterlockedDecrement    // ( &data )
        #define PARALLEL_Increment ::_InterlockedIncrement    // ( &data )
        #define PARALLEL_Exchange ::_InterlockedExchange    // ( &data, value )
        #define PARALLEL_And ::_InterlockedAnd    // ( &data, mask )
        #define PARALLEL_Or ::_InterlockedOr    // ( &data, value )
        #define PARALLEL_Xor ::_InterlockedXor    // ( &data, value )
        #define PARALLEL_ExchangePointer ::InterlockedExchangePointer    // ( &pointer, new_pointer )
        #define PARALLEL_CompareExchange ::InterlockedCompareExchange    // ( &data, value, comparaison_value )
        #define PARALLEL_CompareExchangePointer ::InterlockedCompareExchangePointer    // ( &pointer, value, comparaison_value )
        #define PARALLEL_SignalObjectAndWait ::SignalObjectAndWait
        #define PARALLEL_WaitObject ::WaitForSingleObject
        #define PARALLEL_WaitMultipleObjects ::WaitForMultipleObjects
        #define PARALLEL_Infinite INFINITE
    #endif

#endif