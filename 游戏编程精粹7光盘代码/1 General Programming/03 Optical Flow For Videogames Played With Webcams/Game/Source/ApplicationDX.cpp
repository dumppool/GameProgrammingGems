#include ".\applicationdx.h"
#include <assert.h>
#include "defines.h"
#include <d3dx9.h>
#include "TextureManager.h"
#include "WindowSettings.h"
#include "SoundManager.h"


//--------CONSTRUCTOR & DESTRUCTOR------------------//
CApplicationDX::CApplicationDX(void)
: m_pD3D( NULL )
, m_pD3DDevice( NULL )
, m_hWnd( NULL )
, m_timer(300)
{}

	

CApplicationDX::~CApplicationDX(void)
{
	CTextureManager::GetTextureManager()->CleanUp();
	SoundManager::getInstance().CleanUp();
	CWindowSettings::GetInstance()->CleanUP();
	CHECKED_RELEASE(m_pD3DDevice);
    CHECKED_RELEASE(m_pD3D);
	
}
//----------------------------------------------------//


void CApplicationDX::InitAndLoad(HWND hWnd)
{
	m_hWnd = hWnd;
	InitDX( hWnd ); //Inicializamos todas las variables DirectX
	//assert( SUCCEEDED( hr ) );

	SoundManager::getInstance().load("../Data/XML/sounds.xml");
}

//-----------------------------------------------------------------------------
// Name: InitDX()
// Desc: Initializes DirectX
//-----------------------------------------------------------------------------
HRESULT CApplicationDX::InitDX( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;


	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        

    // Create the D3DDevice D3DCREATE_SOFTWARE_VERTEXPROCESSING D3DDEVTYPE_SW 
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pD3DDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

    // Turn off D3D lighting, since we are providing our own vertex colors
    m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	
	
	
	//Inicializamos los objetos de DirectInput (Keyboard y Mouse)
	
	//InitWindow( hWnd );

	RECT rec_window;
	GetClientRect(	hWnd, &rec_window);
	CWindowSettings::GetInstance()->SetHeight(rec_window.bottom - rec_window.top);
	CWindowSettings::GetInstance()->SetWidth(rec_window.right - rec_window.left);
	
	game.Init(hWnd);
	game.Init(m_pD3DDevice);
	
    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
void CApplicationDX::SetupMatrices()
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;
	{
		D3DXVECTOR3 vEyePt( 0.0f, 0.0f,-5.0f );
		D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
		D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
		m_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

	    
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
		m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	}
	
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );
}


void CApplicationDX::BeginRenderDX()
{
	
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	
    // Begin the scene
    m_pD3DDevice->BeginScene();
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
}
void CApplicationDX::EndRenderDX()
{
	m_pD3DDevice->EndScene();
    // Present the backbuffer contents to the display
    m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}


// En esta funcion compone la escena a pintar, es decir, hace todas las llamadas Render
//de los objetos a dibujar con Direct3D
void CApplicationDX::Render()
{
	m_timer.Update();
	
	BeginRenderDX();
	SetupMatrices();
	//--------------------------------------------------------------------------------------//
	//-------AQUI HACEMOS LAS LLAMADAS DE RENDER DE TODOS LOS ELEMENTOS A DIBUJAR-----------//
	//--------------------------------------------------------------------------------------//
		// >>>> INIT
		game.Draw(m_pD3DDevice, m_timer.GetFPS());
	
		// >>>>> END
	//--------------------------------------------------------------------------------------//		
	EndRenderDX();
}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Update the scene
//-----------------------------------------------------------------------------
bool CApplicationDX::Update()
{
	//Primero de todo calculamos el update del timer
	m_timer.Update();
	float l_ElapsedTime = m_timer.GetElapsedTime();

	game.Update(l_ElapsedTime );

	return game.Exit();
}


