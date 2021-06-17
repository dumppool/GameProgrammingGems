#ifndef INC_WINDOW_ABOUT_H_
#define INC_WINDOW_ABOUT_H_

#include "Window.h"

class CWindow_About: public CWindow
{

public:

	CWindow_About(	const CAnimationImage & newImage, const std::vector<CButton>& buttons,
					unsigned int id):	CWindow(newImage, buttons, id){}

	void	Update	(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
						bool isClicking );
	
	void	Draw	( LPDIRECT3DDEVICE9 device );

	void	Start	( void );

};

#endif //INC_WINDOW_ABOUT_H_