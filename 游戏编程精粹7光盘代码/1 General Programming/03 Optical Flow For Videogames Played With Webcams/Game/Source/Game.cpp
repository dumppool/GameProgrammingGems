#include <assert.h>

#include "Game.h"
#include "TextureManager.h"
#include "AnimationImage.h"
#include "Button.h"

#include "Window_About.h"
#include "Window_Config.h"
#include "Window_Credit.h"
#include "Window_GameOver.h"
#include "Window_Main.h"
#include "Window_Play.h"


CGame::CGame()
: mGameState(eMainWindow)
, mAvailableWebCam( false )
, mTime2ShowNoCamera( 0.f )
{
	
}

CGame::~CGame()
{
	m_mouseInput.FreeDirectInput();
	m_keyboardInput.FreeDirectInput();

	for( std::vector<CWindow*>::iterator it = mWindows.begin(); it != mWindows.end(); ++it )
	{
		switch( (*it)->GetId() )
		{
			case eMainWindow:
				CWindow_Main	* main;
				main = static_cast<CWindow_Main*> (*it);
				delete main;
				break;
			case ePlayWindow:
				CWindow_Play	 * play;
				play = static_cast<CWindow_Play*> (*it);
				delete play;
				break;
			case eGameOverWindow:
				CWindow_GameOver	 * gameOver;
				gameOver = static_cast<CWindow_GameOver*> (*it);
				delete gameOver;
				break;
			case eCreditWindow:
				CWindow_Credit	 * credit;
				credit = static_cast<CWindow_Credit*> (*it);
				delete credit;
				break;
			case eAboutWindow:
				CWindow_About	 * about;
				about = static_cast<CWindow_About*> (*it);
				delete about;
				break;
			case eConfigWindow:
				CWindow_Config	 * config;
				config = static_cast<CWindow_Config*> (*it);
				delete config;
				break;
		}//END switch( (*it)->GetId() )

	}//END for

	mWindows.clear();
}


void CGame::Init( HWND hWnd )
{
	m_keyboardInput.OnCreateDevice(hWnd);
	m_mouseInput.OnCreateDevice(hWnd);

}

void CGame::Draw(  LPDIRECT3DDEVICE9 device, float fps )
{
	std::vector<CWindow*>::iterator it;

	for( it = mWindows.begin(); it != mWindows.end(); ++it )
	{
		if( (*it)->GetId() == mGameState.GetCurrentWindow() )
		{
			break;
		}
	}

	switch( (*it)->GetId() )
	{
		case eMainWindow:
			CWindow_Main	 * main;
			main = static_cast<CWindow_Main*> (*it);
			main->Draw( device);
			break;
		case ePlayWindow:
			CWindow_Play	 * play;
			play = static_cast<CWindow_Play*> (*it);
			play->Draw( device, m_printText2D );
			break;
		case eGameOverWindow:
			CWindow_GameOver	 * gameOver;
			gameOver = static_cast<CWindow_GameOver*> (*it);
			gameOver->Draw( device );
			break;
		case eCreditWindow:
			CWindow_Credit	 * credit;
			credit = static_cast<CWindow_Credit*> (*it);
			credit->Draw( device );
			break;
		case eAboutWindow:
			CWindow_About	 * about;
			about = static_cast<CWindow_About*> (*it);
			about->Draw( device );
			break;
		case eConfigWindow:
			CWindow_Config	 * config;
			config = static_cast<CWindow_Config*> (*it);
			config->Draw( device, m_printText2D );
			break;
	}

	mPointerMouse.Draw(device,1);

	// Codigo de pintado 2d //
	int dy = 580;
	
	dy += m_printText2D.PrintText(TIMES_NEW_ROMAN, 10,dy,0xff880000,"FPS=%f",fps);
	if( IsAvailableWebCam() )
	{
		dy += m_printText2D.PrintText(TIMES_NEW_ROMAN, 10,dy,0xff880000,"WebCamera = OK");
	}
	else
	{
		if( mTime2ShowNoCamera > 0.5 )
			dy += m_printText2D.PrintText(TIMES_NEW_ROMAN, 10,dy,0xff880000,"WebCamera = Camera can't be initialized");
		else
			dy += m_printText2D.PrintText(TIMES_NEW_ROMAN, 10,dy,0xffff0000,"WebCamera = Camera can't be initialized");
	}

}


void CGame::Update( float deltaTime  )
{
	mTime2ShowNoCamera += deltaTime;
	if( mTime2ShowNoCamera > 1.f )
		mTime2ShowNoCamera = 0.f;

	std::vector<CWindow*>::iterator it;
	bool start = false;

	for( it = mWindows.begin(); it != mWindows.end(); ++it )
	{
		if( (*it)->GetId() == mGameState.GetCurrentWindow() )
		{
			start = mGameState.GetStart();
			break;
		}
	}

	m_keyboardInput.ReadImmediateData();
	m_mouseInput.ReadImmediateData();

	
	mPointerMouse.Update( deltaTime, m_mouseInput.GetPosX(),  m_mouseInput.GetPosY() );
	switch( (*it)->GetId() )
	{
		case eMainWindow:
			CWindow_Main	 * main;
			main = static_cast<CWindow_Main*> (*it);
			if(start) main->Start();
			main->Update(	deltaTime, mPointerMouse.GetCenterX(), mPointerMouse.GetCenterY(), mGameState, 
							m_mouseInput.GetButonLeft());
			break;
		case ePlayWindow:
			CWindow_Play	 * play;
			play = static_cast<CWindow_Play*> (*it);
			if(start) play->Start(mGameState);
			play->Update(	deltaTime, mPointerMouse.GetCenterX(), mPointerMouse.GetCenterY(), mGameState, 
							m_mouseInput.GetButonLeft(), m_keyboardInput);
			break;
		case eGameOverWindow:
			CWindow_GameOver	 * gameOver;
			gameOver = static_cast<CWindow_GameOver*> (*it);
			if(start) gameOver->Start();
			gameOver->Update(	deltaTime, mPointerMouse.GetCenterX(), mPointerMouse.GetCenterY(), mGameState,
								m_mouseInput.GetButonLeft());
			break;
		case eCreditWindow:
			CWindow_Credit	 * credit;
			credit = static_cast<CWindow_Credit*> (*it);
			if(start) credit->Start();
			credit->Update(	deltaTime, mPointerMouse.GetCenterX(), mPointerMouse.GetCenterY(), mGameState,
							m_mouseInput.GetButonLeft());
			break;
		case eAboutWindow:
			CWindow_About	 * about;
			about = static_cast<CWindow_About*> (*it);
			if(start) about->Start();
			about->Update(	deltaTime, mPointerMouse.GetCenterX(), mPointerMouse.GetCenterY(), mGameState, 
							m_mouseInput.GetButonLeft() );
			break;
		case eConfigWindow:
			CWindow_Config	 * config;
			config = static_cast<CWindow_Config*> (*it);
			if(start) config->Start();
			config->Update(	deltaTime, mPointerMouse.GetCenterX(), mPointerMouse.GetCenterY(), mGameState,
							m_mouseInput.GetButonLeft());
			break;
	}
}





void CGame::Init( LPDIRECT3DDEVICE9 device )
{
	m_printText2D.SetFonts(device);

	//Load Textures
	LPDIRECT3DTEXTURE9 textDesktop =  CTextureManager::GetTextureManager()->LoadTexture("desktop.tga", device);
	LPDIRECT3DTEXTURE9 textDesktopGameOver =  CTextureManager::GetTextureManager()->LoadTexture("desktop_GameOver.tga", device);
	LPDIRECT3DTEXTURE9 textDesktopCredits =  CTextureManager::GetTextureManager()->LoadTexture("desktop_Credits.tga", device);
	LPDIRECT3DTEXTURE9 textDesktopPlay =  CTextureManager::GetTextureManager()->LoadTexture("desktop_Play.tga", device);
	LPDIRECT3DTEXTURE9 textDesktopAbout =  CTextureManager::GetTextureManager()->LoadTexture("desktop_About.tga", device);
	LPDIRECT3DTEXTURE9 textDesktopConfig =  CTextureManager::GetTextureManager()->LoadTexture("desktop_Config.tga", device);

	
	LPDIRECT3DTEXTURE9 playN =  CTextureManager::GetTextureManager()->LoadTexture("PlayN.tga", device);
	LPDIRECT3DTEXTURE9 playM =  CTextureManager::GetTextureManager()->LoadTexture("PlayM.tga", device);
	LPDIRECT3DTEXTURE9 playC =  CTextureManager::GetTextureManager()->LoadTexture("PlayC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesPlay;
	texturesPlay.push_back(playN); texturesPlay.push_back(playM); texturesPlay.push_back(playC);

	LPDIRECT3DTEXTURE9 pauseN =  CTextureManager::GetTextureManager()->LoadTexture("PauseN.tga", device);
	LPDIRECT3DTEXTURE9 pauseM =  CTextureManager::GetTextureManager()->LoadTexture("PauseM.tga", device);
	LPDIRECT3DTEXTURE9 pauseC =  CTextureManager::GetTextureManager()->LoadTexture("PauseC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesPause;
	texturesPause.push_back(pauseN); texturesPause.push_back(pauseM); texturesPause.push_back(pauseC);

	LPDIRECT3DTEXTURE9 CreditN =  CTextureManager::GetTextureManager()->LoadTexture("CreditsN.tga", device);
	LPDIRECT3DTEXTURE9 CreditM =  CTextureManager::GetTextureManager()->LoadTexture("CreditsM.tga", device);
	LPDIRECT3DTEXTURE9 CreditC =  CTextureManager::GetTextureManager()->LoadTexture("CreditsC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesCredit;
	texturesCredit.push_back(CreditN); texturesCredit.push_back(CreditM); texturesCredit.push_back(CreditC);

	LPDIRECT3DTEXTURE9 ConfigN =  CTextureManager::GetTextureManager()->LoadTexture("ConfigurationN.tga", device);
	LPDIRECT3DTEXTURE9 ConfigM =  CTextureManager::GetTextureManager()->LoadTexture("ConfigurationM.tga", device);
	LPDIRECT3DTEXTURE9 ConfigC =  CTextureManager::GetTextureManager()->LoadTexture("ConfigurationC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesConfig;
	texturesConfig.push_back(ConfigN); texturesConfig.push_back(ConfigM); texturesConfig.push_back(ConfigC);

	LPDIRECT3DTEXTURE9 AboutN =  CTextureManager::GetTextureManager()->LoadTexture("AboutN.tga", device);
	LPDIRECT3DTEXTURE9 AboutM =  CTextureManager::GetTextureManager()->LoadTexture("AboutM.tga", device);
	LPDIRECT3DTEXTURE9 AboutC =  CTextureManager::GetTextureManager()->LoadTexture("AboutC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesAbout;
	texturesAbout.push_back(AboutN); texturesAbout.push_back(AboutM); texturesAbout.push_back(AboutC);

	LPDIRECT3DTEXTURE9 ExitN =  CTextureManager::GetTextureManager()->LoadTexture("ExitN.tga", device);
	LPDIRECT3DTEXTURE9 ExitM =  CTextureManager::GetTextureManager()->LoadTexture("ExitM.tga", device);
	LPDIRECT3DTEXTURE9 ExitC =  CTextureManager::GetTextureManager()->LoadTexture("ExitC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesExit;
	texturesExit.push_back(ExitN); texturesExit.push_back(ExitM); texturesExit.push_back(ExitC);

	LPDIRECT3DTEXTURE9 MenuN =  CTextureManager::GetTextureManager()->LoadTexture("MenuN.tga", device);
	LPDIRECT3DTEXTURE9 MenuM =  CTextureManager::GetTextureManager()->LoadTexture("MenuM.tga", device);
	LPDIRECT3DTEXTURE9 MenuC =  CTextureManager::GetTextureManager()->LoadTexture("MenuC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesMenu;
	texturesMenu.push_back(MenuN); texturesMenu.push_back(MenuM); texturesMenu.push_back(MenuC);

	LPDIRECT3DTEXTURE9 DifficultN =  CTextureManager::GetTextureManager()->LoadTexture("HardN.tga", device);
	LPDIRECT3DTEXTURE9 DifficultM =  CTextureManager::GetTextureManager()->LoadTexture("HardM.tga", device);
	LPDIRECT3DTEXTURE9 DifficultC =  CTextureManager::GetTextureManager()->LoadTexture("HardC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesDifficult;
	texturesDifficult.push_back(DifficultN); texturesDifficult.push_back(DifficultM); texturesDifficult.push_back(DifficultC);

	LPDIRECT3DTEXTURE9 EasyN =  CTextureManager::GetTextureManager()->LoadTexture("EasyN.tga", device);
	LPDIRECT3DTEXTURE9 EasyM =  CTextureManager::GetTextureManager()->LoadTexture("EasyM.tga", device);
	LPDIRECT3DTEXTURE9 EasyC =  CTextureManager::GetTextureManager()->LoadTexture("EasyC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesEasy;
	texturesEasy.push_back(EasyN); texturesEasy.push_back(EasyM ); texturesEasy.push_back(EasyC);

	LPDIRECT3DTEXTURE9 MediumN =  CTextureManager::GetTextureManager()->LoadTexture("MediumN.tga", device);
	LPDIRECT3DTEXTURE9 MediumM =  CTextureManager::GetTextureManager()->LoadTexture("MediumM.tga", device);
	LPDIRECT3DTEXTURE9 MediumC =  CTextureManager::GetTextureManager()->LoadTexture("MediumC.tga", device);
	std::vector<LPDIRECT3DTEXTURE9> texturesMedium;
	texturesMedium.push_back(MediumN); texturesMedium.push_back(MediumM); texturesMedium.push_back(MediumC);
	
	LPDIRECT3DTEXTURE9 PointerMouse =  CTextureManager::GetTextureManager()->LoadTexture("pointer.tga", device);

	CAnimationImage desktop(textDesktop,0,0,100,100);
	CAnimationImage desktopGameOver(textDesktopGameOver,0,0,100,100);
	CAnimationImage desktopCredits(textDesktopCredits,0,0,100,100);
	CAnimationImage desktopPlay(textDesktopPlay,0,0,100,100);
	CAnimationImage desktopAbout(textDesktopAbout,0,0,100,100);
	CAnimationImage desktopConfig(textDesktopConfig,0,0,100,100);

	
	
	
	CButton Play(texturesPlay, 41, 30, 15, 20, 0 , 0, ePlayButton);
	CButton Config(texturesConfig, 28, 45, 15, 50, 0 , 0, eConfigButton);
	CButton Credit(texturesCredit, 39, 60, 15, 25, 0 , 0, eCreditButton);
	CButton About(texturesAbout, 39, 75, 15, 25, 0 , 0, eAboutButton);
	CButton Exit(texturesExit, 80, 85, 15, 20, 0 , 0, eExitButton);

	CButton Menu(texturesMenu, 41, 30, 15, 20, 0 , 0, eMenuButton);
	CButton MenuGameOver(texturesMenu, 41, 80, 15, 20, 0 , 0, eMenuButton);
	CButton MenuConfig(texturesMenu, 41, 85, 15, 20, 0 , 0, eMenuButton);
	CButton MenuAbout(texturesMenu, 41, 85, 15, 20, 0 , 0, eMenuButton);

	CButton Easy(texturesEasy, 10, 25, 15, 20, 0 , 0, eEasyLevelButton);
	CButton Difficult(texturesDifficult, 70, 25, 15, 22, 0 , 0,  eDifficultLevelButton);
	CButton Medium(texturesMedium, 38, 25, 15, 25, 0 , 0,eMediumLevelButton);
	
	CButton MenuPlay(texturesMenu, 28, 72, 15, 20, 0 , 0, eMenuButton);
	CButton Pause(texturesPause, 54, 72, 15, 20, 0 , 0, ePauseButton);
	
	

	std::vector<CButton> buttonsWindowAbout;
	buttonsWindowAbout.push_back(MenuAbout);

	std::vector<CButton> buttonsWindowConfig;
	buttonsWindowConfig.push_back(MenuConfig); 
	buttonsWindowConfig.push_back(Easy);
	buttonsWindowConfig.push_back(Difficult);
	buttonsWindowConfig.push_back(Medium);

	std::vector<CButton> buttonsWindowCredit;
	buttonsWindowCredit.push_back(Menu);

	std::vector<CButton> buttonsWindowGameOver;
	buttonsWindowGameOver.push_back(MenuGameOver);

	std::vector<CButton> buttonsWindowMain;
	buttonsWindowMain.push_back(Play); 
	buttonsWindowMain.push_back(Config);
	buttonsWindowMain.push_back(Credit);
	buttonsWindowMain.push_back(About);
	buttonsWindowMain.push_back(Exit);

	std::vector<CButton> buttonsWindowPlay;
	buttonsWindowPlay.push_back(Pause);
	buttonsWindowPlay.push_back(MenuPlay);

	
	std::vector<LPDIRECT3DTEXTURE9> texturesPointerMouse;
	texturesPointerMouse.push_back(PointerMouse);
	mPointerMouse.SetImages(texturesPointerMouse,6,4);

	CWindow_About *	about;
	about = new CWindow_About(desktopAbout, buttonsWindowAbout, eAboutWindow);
	
	CWindow_Config *	config;
	config = new CWindow_Config(desktopConfig, buttonsWindowConfig, eConfigWindow);

	CWindow_Credit *	credit;
	credit = new CWindow_Credit(desktopCredits , buttonsWindowCredit, eCreditWindow);

	CWindow_GameOver *	gameOver;
	gameOver = new CWindow_GameOver(desktopGameOver, buttonsWindowGameOver, eGameOverWindow);

	CWindow_Main *	main;
	main = new CWindow_Main(desktop, buttonsWindowMain, eMainWindow);

	CWindow_Play *	play;
	play = new CWindow_Play(desktopPlay, buttonsWindowPlay, ePlayWindow, device);
	
	mAvailableWebCam = play->IsAvailableWebCam();				

	mWindows.push_back(about);
	mWindows.push_back(config);
	mWindows.push_back(credit);
	mWindows.push_back(gameOver);
	mWindows.push_back(main);
	mWindows.push_back(play);
}
