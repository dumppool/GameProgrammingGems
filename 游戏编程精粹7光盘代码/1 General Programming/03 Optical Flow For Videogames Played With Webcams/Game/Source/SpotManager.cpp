#include "SpotManager.h"
#include "TextureManager.h"
#include "WindowSettings.h"
#include <math.h>



CSpotManager::CSpotManager(void)
: mNumRegionsX(4)
, mNumRegionsY(4)
, mAverageXScreen(0.6f) // El tamaño en X de la pantalla de juego ocupa el 50% de la pantalla total
, mAverageYScreen(0.5f) // El tamaño en X de la pantalla de juego ocupa el 50% de la pantalla total
, mInitX(0.36f)
, mInitY(0.3f)
, mNumberOfCleaned(0)
, mGameOver(false)
, m_timeToGameOver(0.f)
, m_timeToExplosion( 0.f )
, mStartExplosion(false)
{
	mX_TotalScreen = CWindowSettings::GetInstance()->GetWidth();
	mY_TotalScreen = CWindowSettings::GetInstance()->GetHeight();

	mSizeX = mX_TotalScreen - ( mX_TotalScreen*mAverageXScreen);
	mSizeX = mSizeX/mNumRegionsX;
	
	mSizeY = mY_TotalScreen - ( mY_TotalScreen*mAverageYScreen);
	mSizeY = mSizeY/mNumRegionsY;
}

CSpotManager::~CSpotManager(void)
{
	for(size_t i = 0; i < mVecSpots.size(); i++)
	{
		delete mVecSpots[i];
	}
	mVecSpots.clear();
}

void CSpotManager::Start( CGameState & inGameState )
{
	m_timeToGameOver = 0.f;
	m_timeToExplosion = 0.f;
	mStartExplosion = false;


	int num_regions = mNumRegionsX * mNumRegionsY;
	
	//-------------Spots Vector--------------
	//First we erase mVecSpots
	for(size_t i = 0; i < mVecSpots.size(); i++)
	{
		delete mVecSpots[i];
	}
	mVecSpots.clear();

	//then we fill mVecSpots with the new information
	CSpot * new_spot;
	int seed = (int) timeGetTime();
	srand(seed);
	int idTexture, time2respawn;
	TypeSpot type;

	int pos_radioactive_1 = -1;
	int pos_radioactive_2 = -1;
	if( inGameState.GetLevel() == eMedium )
	{
		pos_radioactive_1 = rand() % 16;		
	}
	else if( inGameState.GetLevel() == eDifficult )
	{
		pos_radioactive_1 = rand() % 16;		
		do
		{
			pos_radioactive_2 = rand() % 16;
		}
		while(pos_radioactive_2 == pos_radioactive_1);
	}
	
	time2respawn = (int)inGameState.GetTime2Respawn();
	for(int i = 0; i < num_regions; i++)
	{
		if( i == pos_radioactive_1 ||  i == pos_radioactive_2) 
		{
			type = eRadioactiveSpot;
			idTexture = 0;
		}
		else
		{
			type = eNormalSpot;
			idTexture = 1 + ( rand() % NUM_TEXTURES );	
		}
		
		int time2Start =  2+ (rand() % 20);
		new_spot = new CSpot(idTexture, (float)time2respawn, (float)time2Start, type);
		mVecSpots.push_back( new_spot );
	}
	//---------------------------------------

    mNumberOfCleaned = 0;
	mGameOver = false;
}
 


int CSpotManager::NumberOfNonCleaned( void )
{
	int numberOfNonCleaned = 0;

	for(size_t cont = 0; cont < mVecSpots.size(); cont++)
	{
		if( mVecSpots[cont]->IsNonCleaned() )
		{
			numberOfNonCleaned ++;
		}
	}

	return numberOfNonCleaned;
}

int CSpotManager::NumberOfCleaned( void )
{
	int numberOfCleaned = 0;
	
	for(size_t cont = 0; cont < mVecSpots.size(); cont++)
	{
		if( mVecSpots[cont]->IsCleaned() )
		{
			numberOfCleaned ++;
			if( mVecSpots[cont]->GetType() == eRadioactiveSpot )
			{
				mStartExplosion = true;
				mVecSpots[cont]->StartExplosion();
			}
		}
	}

	return numberOfCleaned;
}

void CSpotManager::Update(	const float & deltaTime, 
							std::vector<float> & OpticalFlowRegions, 
							std::vector<float> & KeyboardRegions, bool availableWebCam  )
{
	float movement = 0.f;
	float amount = 0.01f;
		
	if( mStartExplosion )
	{
		m_timeToExplosion += deltaTime;
		if( m_timeToExplosion > 2.f )
		{
			mGameOver = true;
			mStartExplosion = false;
		}
	}	
	else
	{
		m_timeToExplosion = 0.f;
	}

	for(size_t cont = 0; cont < mVecSpots.size(); cont++)
	{
		movement = 0.f;

		mVecSpots[cont]->Update(deltaTime);
		

		if( availableWebCam)
		{
			if( OpticalFlowRegions[cont] > 0.3 )
				movement += OpticalFlowRegions[cont] / 4;
		}
		if( KeyboardRegions[cont] == 1 )
		{
			movement += amount;
		}
				
		if( movement != 0.f )
		{
			mVecSpots[cont]->AddValue(movement);
		}
	}

	mNumberOfCleaned += NumberOfCleaned();
	float numberOfSpots = (float) (mNumRegionsX*mNumRegionsY);
	float average = NumberOfNonCleaned() / numberOfSpots;
	if( average == 1.0f)
	{
		m_timeToGameOver += deltaTime;
		if( m_timeToGameOver > 5.f )
			mGameOver = true;
	}
	else
	{
		m_timeToGameOver = 0.f;
	}
}



void CSpotManager::Draw( LPDIRECT3DDEVICE9 device )
{
	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE  );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA  );
	device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	//render...
	int posX, posY;
	for(size_t cont = 0; cont < mVecSpots.size(); cont++)
	{
		float alpha = mVecSpots[cont]->GetAlpha();
		int idTexture =  mVecSpots[cont]->GetIdTexture();

		GetPosition((int)cont, posX, posY);
		DrawQuad(alpha, posX, posY, idTexture, device);
	}
	
	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_TRUE  );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
}


void  CSpotManager::GetPosition(int cont, int& posX, int& posY)
{
	int index_X, index_Y;
	index_X = (int) cont/mNumRegionsY;
	index_Y = cont%mNumRegionsY;

	posX = (int)( (mX_TotalScreen * mInitX) + (index_X*mSizeX) );
	posY = (int)( (mY_TotalScreen * mInitY) + (index_Y*mSizeY) );
}


void CSpotManager::DrawQuad(	const float& alpha, int posX, int posY, 
								int indexTexture,LPDIRECT3DDEVICE9 device )
{
	if( alpha > 0 )
	{
		// render el quad de difuminacion....
		device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); 
		device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); 

		unsigned short l_Indexes[6]={0,2,1,1,2,3};
		/*
		A	/------------------\	C
			|              /   |
			|       POS /      |
			|      / *         |
			| /                |
		B	\------------------/	D*/
		
		D3DXVECTOR3 PointA( posX - mSizeX*0.5f, posY - mSizeY*0.5f, 0.f );
		D3DXVECTOR3 PointB( posX - mSizeX*0.5f, posY + mSizeY*0.5f, 0.f );
		D3DXVECTOR3 PointC( posX + mSizeX*0.5f, posY - mSizeY*0.5f, 0.f );
		D3DXVECTOR3 PointD( posX + mSizeX*0.5f, posY + mSizeY*0.5f, 0.f );
		
		SCREEN_COLOR_VERTEX v[4] = {
				{ PointA.x, PointA.y, PointA.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 0,0}//(x,y) sup_esq.
			,	{ PointB.x, PointB.y, PointB.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 0,1}//(x,y) inf_esq.
			,   { PointC.x, PointC.y, PointC.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 1,0}//(x,y) sup_dr.
			,	{ PointD.x, PointD.y, PointD.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 1,1}//(x,y) inf_dr.
		};		
		device->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
        device->SetTexture(0, GetTexture(indexTexture, device) );
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0, 4, 2, l_Indexes, D3DFMT_INDEX16,v,sizeof(SCREEN_COLOR_VERTEX));
	}
}


LPDIRECT3DTEXTURE9  CSpotManager::GetTexture(int index, LPDIRECT3DDEVICE9 device)
{
	switch(index)
	{
	case 0:
		return CTextureManager::GetTextureManager()->LoadTexture("radioactive.png", device);
	case 1:
		return CTextureManager::GetTextureManager()->LoadTexture("spot1.1.tga", device);
	case 2:
		return CTextureManager::GetTextureManager()->LoadTexture("spot1.2.tga", device);
	case 3:
		return CTextureManager::GetTextureManager()->LoadTexture("spot1.3.tga", device);
	case 4:
		return CTextureManager::GetTextureManager()->LoadTexture("spot1.4.tga", device);
	case 5:
		return CTextureManager::GetTextureManager()->LoadTexture("spot1.5.tga", device);
	case 6:
		return CTextureManager::GetTextureManager()->LoadTexture("spot1.6.tga", device);
	case 7:
		return CTextureManager::GetTextureManager()->LoadTexture("spot2.1.tga", device);
	case 8:
		return CTextureManager::GetTextureManager()->LoadTexture("spot2.2.tga", device);
	case 9:
		return CTextureManager::GetTextureManager()->LoadTexture("spot2.3.tga", device);
	case 10:
		return CTextureManager::GetTextureManager()->LoadTexture("spot3.1.tga", device);
	case 11:
		return CTextureManager::GetTextureManager()->LoadTexture("spot3.2.tga", device);
	case 12:
		return CTextureManager::GetTextureManager()->LoadTexture("spot3.3.tga", device);

	case 13:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_1.tga", device);
	case 14:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_2.tga", device);
	case 15:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_3.tga", device);
	case 16:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_4.tga", device);
	case 17:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_5.tga", device);
	case 18:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_6.tga", device);
	case 19:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_7.tga", device);
	case 20:
		return CTextureManager::GetTextureManager()->LoadTexture("rlboom_8.tga", device);
	}
	
	return 0;
}

