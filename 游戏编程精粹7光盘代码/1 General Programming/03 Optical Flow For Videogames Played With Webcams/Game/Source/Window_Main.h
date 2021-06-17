#ifndef INC_WINDOW_MAIN_H_
#define INC_WINDOW_MAIN_H_

#include "Window.h"

class CWindow_Main: public CWindow
{

public:

	CWindow_Main(	const CAnimationImage & newImage, const std::vector<CButton>& buttons,
					unsigned int id):	CWindow(newImage, buttons, id), mInit(true){}

	void	Update	(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
						bool isClicking );

	void	Draw	( LPDIRECT3DDEVICE9 device );

	void	Start	( void );

private:

	bool	mInit;
};

#endif //INC_WINDOW_MAIN_H_