#ifndef INC_SPOTMANAGER_H
#define INC_SPOTMANAGER_H

#include <vector>
#include "Spot.h"
#include "defines.h"
#include "GameState.h"

#define NUM_TEXTURES 12



class CSpotManager
{

public:

	CSpotManager(void);
	~CSpotManager(void);

	void	Update			(	const float & deltaTime,  std::vector<float> & OpticalFlowRegions, 
								std::vector<float> & KeyboardRegions, bool availableWebCam );

	void	Draw			( LPDIRECT3DDEVICE9 mDevice );

	void	Start			( CGameState & inGameState ); 

	int		GetNumOfCleaned	( void ) {return mNumberOfCleaned;}

	bool	IsGameOver		( void ) {return mGameOver;}

private:

	float					mTime;
	std::vector<CSpot*>		mVecSpots;
	
	int		mX_TotalScreen;
	int		mY_TotalScreen;
	float	mSizeX;
	float	mSizeY;
	float	mAverageXScreen;
	float	mAverageYScreen;
	int		mNumRegionsX;
	int		mNumRegionsY;
	float	mInitX;
	float	mInitY;
	int		mNumberOfCleaned;
	
	bool	mGameOver;
	float	m_timeToGameOver;
	
	float	m_timeToExplosion;
	bool	mStartExplosion;


private:

	void	GetPosition			( int cont, int& posX, int& posY );

	int		NumberOfCleaned		( void );

	int		NumberOfNonCleaned	( void );

	void	DrawQuad			(	const float& alpha, int posX, int posY,
									int indexTexture,LPDIRECT3DDEVICE9 mDevice );

	LPDIRECT3DTEXTURE9 	GetTexture	( int index, LPDIRECT3DDEVICE9 device );
};

#endif //INC_SPOTMANAGER_H
