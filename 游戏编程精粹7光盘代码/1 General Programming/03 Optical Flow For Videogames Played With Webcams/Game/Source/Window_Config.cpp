#include "Window_Config.h"
#include "defines.h"


void CWindow_Config::Update(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
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

			case eEasyLevelButton:
				{
					inGameState.SetLevel(eEasy);
				}
				break;

			case eMediumLevelButton:
				{
					inGameState.SetLevel(eMedium);
				}
				break;
			
			case eDifficultLevelButton:
				{
					inGameState.SetLevel(eDifficult);
				}
				break;
		}
	}
	mLevel = inGameState.GetLevel();
}

void CWindow_Config::Draw( LPDIRECT3DDEVICE9 device, CDebugPrintText2D & text2d  )
{
	CWindow::Draw(device);

	int dy = 560;
	switch (mLevel)
	{
		case eEasy:
			dy += text2d.PrintText(TIMES_NEW_ROMAN, 10,dy,0xff880000,"Level = Easy");
			break;
		case eMedium:
			dy += text2d.PrintText(TIMES_NEW_ROMAN, 10,dy,0xff880000,"Level = Normal");
			break;
		case eDifficult:
			dy += text2d.PrintText(TIMES_NEW_ROMAN, 10,dy,0xff880000,"Level = Difficult");
			break;
	}
	
}

void CWindow_Config::Start( void )
{
}