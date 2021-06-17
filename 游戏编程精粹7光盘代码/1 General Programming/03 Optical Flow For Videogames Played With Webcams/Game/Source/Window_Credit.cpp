#include "Window_Credit.h"
#include "defines.h"


void CWindow_Credit::Update(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
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

void CWindow_Credit::Draw( LPDIRECT3DDEVICE9 device  )
{
	CWindow::Draw(device);
}

void CWindow_Credit::Start( void )
{
}