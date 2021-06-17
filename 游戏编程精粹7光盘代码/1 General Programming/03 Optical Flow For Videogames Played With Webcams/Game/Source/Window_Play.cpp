#include "Window_Play.h"
#include "defines.h"
#include "TextureManager.h"


CWindow_Play::CWindow_Play(	const CAnimationImage & newImage, const std::vector<CButton>& buttons, 
							unsigned int id, LPDIRECT3DDEVICE9 device)
: CWindow(newImage, buttons, id)
, m_webCamera(true, 1)
, m_WebCamTexture( 0 )
, m_timeToUpdateWebCam( 0.f )
, m_pause( false )
, m_AvailableWebCam( false )
, m_time(0)
{
	m_AvailableWebCam = m_webCamera.IsAvailableWebCam();
	if( m_AvailableWebCam )
	{
		int width, height;
		m_webCamera.GetSizeImage( width, height);
		for(int col = 0; col < 4; col++)
		{
			for(int fil = 0; fil < 4; fil++)
			{
				m_webCamera.AddRegion(col* (width/4), fil* (height/4), width/4, height/4);
			}
		}
		HRESULT hr;
		hr = device->CreateTexture(	width, height, 1, D3DUSAGE_DYNAMIC,
									D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT, &m_WebCamTexture, NULL);
	}
	else
	{
		m_WebCamTexture = CTextureManager::GetTextureManager()->LoadTexture("noWebCam.tga", device);
		//legim textura para indicar que la webcamara nones de nones...
	}
}

CWindow_Play::~CWindow_Play()
{
	if( m_AvailableWebCam )
	{
		m_webCamera.DeInit();
	}
}

void CWindow_Play::Update(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
							bool isClicking, CKeyboardInput & keyboard)
{
	CWindow::Update(deltaTime, posXMouse,posYMouse,isClicking);

	if( mIsClicked )
	{
		switch( mButtonClicked )
		{
			case eMenuButton:
				{
					inGameState.SetCurrentWindow(eMainWindow);
				}
				break;
			case ePauseButton:
				{
					m_pause = !m_pause;
				}
				break;
		}
	}

	//Update SpotManager
	if( !m_pause )
	{
		m_time += deltaTime;
		std::vector<float>	opticalFlowRegions;
		if( m_AvailableWebCam )
		{
			m_timeToUpdateWebCam += deltaTime;
			if( m_timeToUpdateWebCam > 0.5 )
			{
				m_timeToUpdateWebCam = 0.f;
				m_webCamera.QueryFlow();
			}
			m_webCamera.FlowRegions( opticalFlowRegions );
		}
		
		std::vector<float>	KeyboardRegions;
		GetKeyboardRegions( KeyboardRegions, keyboard );
		
		
		m_spotManager.Update(deltaTime, opticalFlowRegions, KeyboardRegions, m_AvailableWebCam);

		if( m_spotManager.IsGameOver() )
		{
			inGameState.SetCurrentWindow(eGameOverWindow);
		}
	}
}

void CWindow_Play::Draw( LPDIRECT3DDEVICE9 device, CDebugPrintText2D & text2d  )
{
	CWindow::Draw(device);
	DrawTextureFromWebCam( device );
	m_spotManager.Draw( device ); 

	int numberOfCleaned = m_spotManager.GetNumOfCleaned();
	text2d.PrintText(SNAPITC, 510,560,0xff660000,"%d",numberOfCleaned);
	text2d.PrintText(SNAPITC, 710,560,0xff660000,"%d",(int) m_time);
}



void CWindow_Play::DrawTextureFromWebCam( LPDIRECT3DDEVICE9 device )
{
	if( m_AvailableWebCam )
	{
		GetTextureFromWebCam();
	}
	float alpha = 1.f;
	unsigned short l_Indexes[6]={0,2,1,1,2,3};
	/*
		A	/------------------\	C
			|              /   |
			|       POS /      |
			|      / *         |
			| /                |
		B	\------------------/	D*/
	
	D3DXVECTOR3 PointA( 250.f, 150.f, 0.f );
	D3DXVECTOR3 PointB( 250.f, 450.f, 0.f );
	D3DXVECTOR3 PointC( 550.f, 150.f, 0.f );
	D3DXVECTOR3 PointD( 550.f, 450.f, 0.f );
	
	SCREEN_COLOR_VERTEX v[4] = {
			{ PointA.x, PointA.y, PointA.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 0,0}//(x,y) sup_esq.
		,	{ PointB.x, PointB.y, PointB.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 0,1}//(x,y) inf_esq.
		,   { PointC.x, PointC.y, PointC.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 1,0}//(x,y) sup_dr.
		,	{ PointD.x, PointD.y, PointD.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 1,1}//(x,y) inf_dr.
	};		
	device->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
	device->SetTexture(0,m_WebCamTexture);
	device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0, 4, 2, l_Indexes, D3DFMT_INDEX16,v,sizeof(SCREEN_COLOR_VERTEX));

}


HRESULT CWindow_Play::GetTextureFromWebCam( void )
{

	int width, height;
	m_webCamera.GetSizeImage( width, height);
	D3DLOCKED_RECT lock;

	HRESULT hr;
	hr=m_WebCamTexture->LockRect(0,&lock,0,D3DLOCK_DISCARD);
	if(hr==D3D_OK)
	{
		char *pTxtBuffer; // Bitmap buffer, texture buffer
		uchar *pImgBuffer; // Bitmap buffer, texture buffer
		pTxtBuffer = (char*)lock.pBits;
		pImgBuffer = (uchar*)m_webCamera.GetImageForRender();
		
		int j = 0;
		for( int cont = 0; cont< width * height; cont++)
		{
			//BLUE
			pTxtBuffer[cont*4 + 0] = pImgBuffer[j]; j++;
			//GREEN
			pTxtBuffer[cont*4 + 1] = pImgBuffer[j]; j++; 			
			//RED
			pTxtBuffer[cont*4 + 2] = pImgBuffer[j]; j++;
			//ALPHA
			pTxtBuffer[cont*4 + 3] = 255;
		}
	 hr=m_WebCamTexture->UnlockRect(0);
	}
	return hr;
}

void CWindow_Play::Start( CGameState & inGameState )
{
	m_spotManager.Start( inGameState );
	m_time = 0.f;
}

void CWindow_Play::GetKeyboardRegions(  std::vector<float> & inKeyboardRegions, CKeyboardInput & keyboard  )
{
	char * keys = keyboard.GetKey();
	
	if( KEYDOWN(keys, DIK_1) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_Q) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_A) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_Z) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_2) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_W) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_S) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_X) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_3) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_E) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_D) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_C) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_4) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_R) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_F) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);

	if( KEYDOWN(keys, DIK_V) )
		inKeyboardRegions.push_back(1);
	else
		inKeyboardRegions.push_back(0);
}


