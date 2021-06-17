#ifndef INC_GAMESTATE_H_
#define INC_GAMESTATE_H_


enum TypeLevel { eEasy, eMedium, eDifficult };

class CGameState
{

public:

	CGameState( unsigned int idCurrentWindow );

	unsigned int	GetCurrentWindow	( void ) const {return mIdCurrentWinwow;}
	void			SetCurrentWindow	( unsigned int id );

	bool		GetExit	( void ) const {return mExit;}
	void		SetExit	( bool flag ) { mExit = flag; }

	TypeLevel	GetLevel	( void ) const {return mLevel;}
	void		SetLevel	( TypeLevel level ) { mLevel = level; }

	bool		GetStart	( void );

	float		GetTime2Respawn	( void );

	
private:

	bool			mStart;
	bool			mExit;
	TypeLevel		mLevel;
	unsigned int	mIdCurrentWinwow;

	float	mTime2Respawn;
	
	
};

#endif //INC_GAMESTATE_H_