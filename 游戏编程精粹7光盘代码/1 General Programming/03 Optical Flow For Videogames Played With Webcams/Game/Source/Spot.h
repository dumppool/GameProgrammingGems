#ifndef INC_SPOT_H
#define INC_SPOT_H


enum TypeSpot { eNormalSpot, eRadioactiveSpot }; 

class CSpot
{

public:
	CSpot(int idTexture, const float& time2Respawn, const float& time2Start, TypeSpot type);
	~CSpot(void);


	void	AddValue		( const float &amount );
	void	AddTime2Respawn	( const float &amount );
	void	Update			( const float &deltaTime );

	bool	IsCleaned		( void );
	bool	IsNonCleaned	( void ) const;
	int		GetIdTexture	( void ) {return mIdTexture;}
	float&	GetAlpha		( void ) {return mAlpha;}
	void	StartExplosion	( void );

	TypeSpot	GetType		( void ) {return mType;}
	


private:

	TypeSpot mType;

	float	mAlpha;
	int		mIdTexture;
	bool	mInit;
	
	float	mTime;
	float	mTime2Respawn;
	float	mTime2Start;
	float	mTime2ExplosionTexture;
	bool	mInitRespawn;
	bool	mIsClean;

	bool	mStartExplosion;
	bool	mEndExplosionAnimation;
};

#endif //INC_SPOT_H