#include "Window_GameOver.h"
#include "defines.h"
#include "SoundManager.h"


void CWindow_GameOver::Update(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
								bool isClicking)
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

void CWindow_GameOver::Draw( LPDIRECT3DDEVICE9 device  )
{
	CWindow::Draw(device);
}

void CWindow_GameOver::Start( void )
{
	SoundManager::getInstance().PlaySource("gameOver");
}