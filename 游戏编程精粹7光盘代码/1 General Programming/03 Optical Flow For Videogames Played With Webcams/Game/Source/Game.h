#ifndef INC_GAME_H_
#define INC_GAME_H_

#include <vector>
#include "Window.h"
#include "GameState.h"
#include "PointerMouse.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "DebugPrinttext2D.h"

class CGame
{

public:

	CGame();
	~CGame();

	void	Init		( HWND hWnd );
	void	Init		( LPDIRECT3DDEVICE9 device );
	void	Draw		( LPDIRECT3DDEVICE9 device, float fps );
	void	Update		( float deltaTime  );
	bool	Exit		( void ) const {return mGameState.GetExit();}

	bool	IsAvailableWebCam	( void ) {return mAvailableWebCam;}

private:

	CDebugPrintText2D	m_printText2D;	//Objeto para poder pintar texto2D en pantalla

	std::vector<CWindow*>	mWindows;
	CGameState				mGameState;
	CPointerMouse			mPointerMouse;
	bool					mAvailableWebCam;

	float	mTime2ShowNoCamera;

	CMouseInput		m_mouseInput;		//Objeto con la informacion relacionada con el Mouse
	CKeyboardInput	m_keyboardInput;	//Objeto con la informacion relacionada con el Keyboard

};

#endif //INC_GAME_H_