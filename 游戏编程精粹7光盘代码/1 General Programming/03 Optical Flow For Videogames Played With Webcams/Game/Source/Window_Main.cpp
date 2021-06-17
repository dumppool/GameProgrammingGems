#include "Window_Main.h"
#include "defines.h"
#include "SoundManager.h"


void CWindow_Main::Update(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
							bool isClicking)
{
	CWindow::Update(deltaTime, posXMouse,posYMouse,isClicking);

	
	if( mIsClicked )
	{
		switch( mButtonClicked )
		{
			case ePlayButton:
				{
					inGameState.SetCurrentWindow(ePlayWindow);
				}
				break;
			case eExitButton:
				{
					inGameState.SetExit(true);
				}
				break;
			case eCreditButton:
				{
					inGameState.SetCurrentWindow(eCreditWindow);
				}
				break;
			case eConfigButton:
				{
					inGameState.SetCurrentWindow(eConfigWindow);
				}
				break;
			case eAboutButton:
				{
					inGameState.SetCurrentWindow(eAboutWindow);
				}
				break;
		}
	}
}

void CWindow_Main::Draw( LPDIRECT3DDEVICE9 device)
{
	CWindow::Draw(device);
}


void CWindow_Main::Start( void )
{
	if( mInit == true )
	{
		mInit = false;
		SoundManager::getInstance().PlaySource("init");
	}
}