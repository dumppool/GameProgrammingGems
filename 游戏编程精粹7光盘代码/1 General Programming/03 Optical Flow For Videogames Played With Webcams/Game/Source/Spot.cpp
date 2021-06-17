#include "Spot.h"
#include "SoundManager.h"

CSpot::CSpot(int idTexture, const float& time2Respawn, const float& time2Start, TypeSpot type)
: mAlpha(0.f)
, mIdTexture(idTexture)
, mTime(0.f)
, mInitRespawn(false)
, mTime2Respawn(time2Respawn)
, mInit(true)
, mTime2Start(time2Start)
, mIsClean(false)
, mType( type )
, mStartExplosion(false)
, mEndExplosionAnimation(false)
, mTime2ExplosionTexture(0.f)
{}

CSpot::~CSpot(void)
{
}


bool CSpot::IsCleaned( void ) 
{
	bool isClean = mIsClean;
	mIsClean = false;
	return isClean;
}
	
bool CSpot::IsNonCleaned( void ) const
{
	bool isNonClean = false;

	if( mAlpha != 0 && !mInit)
	{
		isNonClean = true;
	}
	
	return isNonClean;
}

void CSpot::AddTime2Respawn( const float &amount )
{
	mTime2Respawn -= amount;
}

void CSpot::AddValue( const float &amount )
{
	if( !mStartExplosion )
	{
		mAlpha -= amount;
		if( mAlpha < 0.f )
		{
			mAlpha = 0.f;
		}
		if( mAlpha > 1.f )
		{
			mAlpha = 1.f;
		}
	}
}

void CSpot::StartExplosion( void )
{
	mStartExplosion = true; 
	mIdTexture = 13;
	//Sound of explosion
	SoundManager::getInstance().PlaySource("boom");
	mTime = 0.f;
	mInitRespawn = false;
	mAlpha = 1.f;
}


void CSpot::Update( const float & deltaTime )
{
	if( mStartExplosion )
	{
		mTime2ExplosionTexture += deltaTime;
		if( mTime2ExplosionTexture > 0.1f &&  !mEndExplosionAnimation)
		{
			mTime2ExplosionTexture = 0.f;
			mIdTexture++;
			if( mIdTexture > 20 )
			{
				mEndExplosionAnimation = true;
				mIdTexture = 0;
			}
		}
	}

	else
	{
		if(mInit)
		{
			mTime += deltaTime;
			if( mTime > mTime2Start)
			{
				mTime = 0.f;
				mInit = false;
				mAlpha = 1.f;
				//The sound of respawn
				SoundManager::getInstance().PlaySource("splash");
			}
		}
		else
		{
			if( mAlpha == 0.f && !mInitRespawn)
			{
				mIsClean = true;
				mInitRespawn = true;
			}

			if( mInitRespawn == true )
			{
				mTime += deltaTime;
				if( mTime > mTime2Respawn )
				{
					mTime = 0.f;
					mInitRespawn = false;
					mAlpha = 1.f;
					//The sound of respawn
					SoundManager::getInstance().PlaySource("splash");
				}
			}
		}
	}
}