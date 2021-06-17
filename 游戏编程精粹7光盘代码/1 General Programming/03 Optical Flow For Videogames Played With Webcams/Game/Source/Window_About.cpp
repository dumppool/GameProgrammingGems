#include "Window_About.h"
#include "defines.h"


void CWindow_About::Update(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
							bool isClicking )
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
		}
	}
}

void CWindow_About::Draw( LPDIRECT3DDEVICE9 device  )
{
	CWindow::Draw(device);
}

void CWindow_About::Start( void )
{
}