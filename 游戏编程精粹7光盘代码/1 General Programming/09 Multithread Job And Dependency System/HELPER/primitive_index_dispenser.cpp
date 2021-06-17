#include "primitive_index_dispenser.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include <assert.h>

// -- PUBLIC

// .. OPERATIONS

void PRIMITIVE_INDEX_DISPENSER::FreeAllIndexes(
    void
    )
{
    int
        free_index;

    assert( IndexCount > 0 );

    FirstFreeIndex = 0;

    for( free_index = 0; free_index < IndexCount - 1; ++free_index )
    {
        NextFreeIndexTable[ free_index ] = free_index + 1 ;
    }

    NextFreeIndexTable[ IndexCount - 1 ] = -1;
}

// ~~

void PRIMITIVE_INDEX_DISPENSER::SetIndexCount(
    const int index_count
    )
{
    IndexCount = index_count;
    NextFreeIndexTable.resize( IndexCount );

    FreeAllIndexes();
}

// ~~

int PRIMITIVE_INDEX_DISPENSER::GetFreeIndex(
    void
    )
{
    int
        free_index;

    assert( FirstFreeIndex != -1 );

    free_index = FirstFreeIndex;
    FirstFreeIndex = NextFreeIndexTable[ free_index ];
    NextFreeIndexTable[ free_index ] = 0;

    return free_index;
}

// ~~

void PRIMITIVE_INDEX_DISPENSER::FreeIndex(
    const int used_index
    )
{
    assert( NextFreeIndexTable[ used_index ] == 0 );

    NextFreeIndexTable[ used_index ] = FirstFreeIndex;
    FirstFreeIndex = used_index;
}

