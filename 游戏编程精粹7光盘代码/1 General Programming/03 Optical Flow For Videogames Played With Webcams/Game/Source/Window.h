#ifndef INC_WINDOW_H_
#define INC_WINDOW_H_

#include <vector>
#include "Button.h"
#include "AnimationImage.h"
#include "GameState.h"
#include "KeyboardInput.h"
#include "DebugPrintText2D.h"


class CWindow
{

public:

	CWindow( const CAnimationImage & newImage, const std::vector<CButton>& buttons, unsigned int id);
			
	void			Draw		( LPDIRECT3DDEVICE9 device );
	
	void			Update		(	float deltaTime, int posXMouse, int posYMouse, bool isClicking );
	
	void			AddButton	( CButton newButton ) {mButtons.push_back(newButton);}
	
	unsigned int	GetId		( void ) const {return mIdWindow;}

protected:
	std::vector<CButton>	mButtons;
	CAnimationImage			mImage;
	bool					mIsClicked;
	unsigned int			mButtonClicked;
	unsigned int			mIdWindow;
	
	

};

#endif //INC_WINDOW_H_