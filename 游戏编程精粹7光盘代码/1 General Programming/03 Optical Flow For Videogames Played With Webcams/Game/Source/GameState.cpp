#include "GameState.h"

CGameState::CGameState( unsigned int idCurrentWindow )
: mIdCurrentWinwow( idCurrentWindow )
, mLevel( eEasy )
, mExit( false )
, mStart( true )
, mTime2Respawn( 5 )
{}

bool CGameState::GetStart( void )
{
	if( mStart )
	{
		mStart = false;
		return true;
	}
	else return false;
}

void CGameState::SetCurrentWindow( unsigned int id )
{
	if( mIdCurrentWinwow != id )
	{
		mStart = true;
	}
	
	mIdCurrentWinwow = id;
}

float CGameState::GetTime2Respawn( void )
{
	switch(mLevel)
	{
		case eEasy:
			return 10.f;
		case eMedium:
			return 6.f;
		case eDifficult:
			return 3.f;
	}
	return 0.f;
}