
#include <Windows.h>
#include "ApplicationDX.h"


#define APPLICATION_NAME				"Game Programming Gems 7: Game with Optical Flow"


//---------PARA DETECTAR MEMORY LEAKS--------//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//-------------------------------------------//



//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
		case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	//-----------Para detectar Memory Leaks-------------------------
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc (54);
	//---------------------------------------------------------------//


	// Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      APPLICATION_NAME, NULL };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( APPLICATION_NAME, APPLICATION_NAME,
                              WS_OVERLAPPEDWINDOW, 100, 100, 800, 640,
                              NULL, NULL, wc.hInstance, NULL );

	

	CApplicationDX l_ApplicationDX;
	l_ApplicationDX.InitAndLoad(hWnd);
	ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );
	MSG msg;
    ZeroMemory( &msg, sizeof(msg) );

	while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
		{
			if( l_ApplicationDX.Update() )
			{
				break;
			}
			l_ApplicationDX.Render();	
		}
	}
	UnregisterClass( APPLICATION_NAME, wc.hInstance );
	return 0;
}



