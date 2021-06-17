// -----------------------------------------------------------
// KdTreeDemo - template.cpp
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#include "surface.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include "game.h"

static int SCRPITCH = 0;
static bool FULLSCREEN = false;

typedef struct _D3DCurrentSettings
{
	int height, width;
	BOOL windowed;
	BOOL multisampling;
	D3DFORMAT d3dformat;
	D3DPRESENT_PARAMETERS presentparams;
} D3DCURRENTSETTINGS;

class D3DRenderer
{
public:
	void Settings();
	HRESULT Init();
	void Render();
	void Screenshot();
	HRESULT Tick();
	// data members
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 device;
	D3DCURRENTSETTINGS settings;
	LPDIRECT3DSURFACE9 bsurf;
};

static char appname[] = "KdTreeDemo";
HWND hWnd;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddevice;
LPDIRECT3DSURFACE9 bsurf;
D3DCURRENTSETTINGS d3dsettings;
bool exitapp = false;
D3DRenderer renderer;
using namespace KdTreeDemo;
Surface* surface = 0;
Pixel* buffer = 0;
Game* game = 0;
bool initialized = false;
float fps = 30;
double lastftime = 0;

int InitD3DDevice( HWND w, D3DCURRENTSETTINGS& cs, LPDIRECT3D9 d3d, LPDIRECT3DDEVICE9* device )
{
	D3DPRESENT_PARAMETERS pp;
	D3DDISPLAYMODE dm;
	if (*device) (*device)->Release();
	ZeroMemory( &pp, sizeof( D3DPRESENT_PARAMETERS ) );
	HRESULT res = d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &dm );
	if (FAILED( res )) return E_FAIL;
	pp.BackBufferWidth = cs.width, pp.BackBufferHeight = cs.height;
	pp.BackBufferFormat = cs.windowed ? dm.Format : cs.d3dformat;
	pp.hDeviceWindow = w;
	pp.Windowed = cs.windowed;
	pp.BackBufferCount = 1;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	if (cs.multisampling)
	{
		res = d3d->CheckDeviceMultiSampleType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pp.BackBufferFormat, cs.windowed, D3DMULTISAMPLE_2_SAMPLES, NULL );
		if( SUCCEEDED( res )) pp.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; // doesn't work?
	}
	pp.EnableAutoDepthStencil = TRUE;
	pp.AutoDepthStencilFormat = D3DFMT_D16;
	pp.FullScreen_RefreshRateInHz = 0;
	pp.PresentationInterval = cs.windowed ? 0 : D3DPRESENT_INTERVAL_IMMEDIATE;
	pp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	res = d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, device );
	if (FAILED( res )) res = d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, w, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, device );
	if (FAILED( res )) return E_FAIL;
	cs.presentparams = pp;
	return S_OK;
}

HRESULT ValidateDevice( LPDIRECT3DDEVICE9& device, LPDIRECT3DSURFACE9& bsurf, D3DCURRENTSETTINGS& cs )
{
	HRESULT res = device->TestCooperativeLevel();
	if (FAILED( res ))
	{
		if (res == D3DERR_DEVICELOST) return E_FAIL;
		if (res == D3DERR_DEVICENOTRESET)
		{
			if (bsurf) bsurf->Release();
			if (FAILED( res = device->Reset( &cs.presentparams ) ))
			{
				exitapp = true;
				return E_FAIL;
			}
			if (FAILED( device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &bsurf ) ))
			{
				exitapp = true;
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT D3DInit()
{
	renderer.Settings();
	HRESULT res = 0;
	if ((d3d = Direct3DCreate9( D3D_SDK_VERSION )) == NULL) return E_FAIL;
	res = InitD3DDevice( hWnd, d3dsettings, d3d, &d3ddevice );
	if (FAILED( res )) return res; else return S_OK;
}

HRESULT D3DShutdown()
{
	if (d3ddevice) d3ddevice->Release();
	if (bsurf) bsurf->Release();
	if (d3d) d3d->Release();
	return S_OK;
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HDC hDC;
	PAINTSTRUCT ps;
	switch( msg )
	{
	case WM_PAINT:
		hDC = BeginPaint( hWnd, &ps );
		EndPaint( hWnd, &ps );
		break;
	case WM_KEYDOWN:
		if (wParam != VK_ESCAPE) break;
	case WM_QUIT:
		exitapp = true;
		break;
	default: 
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( WNDCLASSEX ) );
	wc.cbSize = sizeof( WNDCLASSEX );						// size of the window struct in bytes
	wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;				// window styles to use
	wc.lpfnWndProc = MsgProc;								// function name of event handler
	wc.hInstance = hInstance;								// handle to this apps instance
	wc.hbrBackground = (HBRUSH)GetStockObject( GRAY_BRUSH );// background colour of window
	wc.hIcon= LoadIcon( NULL, IDI_APPLICATION );			// icon for the app window
	wc.hIconSm=LoadIcon( NULL, IDI_APPLICATION );			// icon when minimized to taskbar
	wc.hCursor=LoadCursor( NULL, IDC_ARROW );				// cursor to use for this window
	wc.lpszClassName = appname;								// name for this class
	RegisterClassEx( &wc );
	renderer.Settings();
	d3dsettings = renderer.settings;
	// Create and show the application's window
	hWnd = CreateWindow( appname, appname, WS_POPUP, 10, 10, 
		d3dsettings.width, d3dsettings.height, NULL, NULL, wc.hInstance, NULL );
	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
	if (FAILED( D3DInit() ))
	{
		UnregisterClass( appname, wc.hInstance );
		return -1;
	}
	else 
	{
		renderer.d3d = d3d;
		renderer.device = d3ddevice;
		renderer.bsurf = bsurf;
		renderer.Init();
	}
	// Enter the message loop
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	while (!exitapp)
	{
		if (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else 
		{
			if (d3ddevice) 
			{
				HRESULT res = ValidateDevice( d3ddevice, bsurf, d3dsettings );
				if (SUCCEEDED( res )) renderer.Tick();
			}
		}
	}
	D3DShutdown();
	ExitProcess( 0 ); // DestroyWindow( hWnd );
	return 0;
}

void D3DRenderer::Settings()
{
	settings.windowed = FULLSCREEN?FALSE:TRUE;
    settings.multisampling = FALSE;
    settings.d3dformat = D3DFMT_X8R8G8B8;
	settings.width = SCRWIDTH;
    settings.height = SCRHEIGHT;
}

HRESULT D3DRenderer::Init()
{
   	surface = new Surface( SCRWIDTH, SCRHEIGHT, 0, SCRWIDTH );
	surface->InitCharset();
	return S_OK;
}

void D3DRenderer::Render()
{
	if (!initialized)
	{
		game = new Game();
	   	surface->Clear( 0 );
		game->SetTarget( surface );
		initialized = true;
	}
	else game->Tick();
}

HRESULT D3DRenderer::Tick()
{
	device->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &bsurf );
	D3DLOCKED_RECT rect;
	bsurf->LockRect( &rect, NULL, 0 );
	DWORD* data = (DWORD*)(rect.pBits);
	SCRPITCH = rect.Pitch / 4;
	surface->SetBuffer( data );
	surface->SetPitch( SCRPITCH );
	Render();
	bsurf->UnlockRect();
	bsurf->Release();
	device->Present( NULL, NULL, NULL, NULL );
	return S_OK;
}

Surface* GetSurface()
{
	return surface;
}