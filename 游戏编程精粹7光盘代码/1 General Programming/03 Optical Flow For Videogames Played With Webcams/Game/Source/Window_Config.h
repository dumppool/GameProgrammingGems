#ifndef INC_WINDOW_CONFIG_H_
#define INC_WINDOW_CONFIG_H_

#include "Window.h"

class CWindow_Config: public CWindow
{

public:

	CWindow_Config(	const CAnimationImage & newImage, const std::vector<CButton>& buttons,
					unsigned int id):	CWindow(newImage, buttons, id){}

	void	Update	(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
						bool isClicking );
	
	void	Draw	( LPDIRECT3DDEVICE9 device, CDebugPrintText2D & text2d  );
	
	void	Start	( void );

private:
	TypeLevel	mLevel;
};

#endif //INC_WINDOW_CONFIG_H_