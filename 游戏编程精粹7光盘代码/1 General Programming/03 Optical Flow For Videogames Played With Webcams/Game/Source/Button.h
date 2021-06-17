#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <vector>
#include "AnimationImage.h"


enum TypeImage { eNormal, eMouseOver, eClicked };

class CButton
{

public:

	CButton(	const std::vector<LPDIRECT3DTEXTURE9> & textures, int posX, int posY, 
				int h, int W, int soundClick, int soundOverMouse, unsigned int idButton );
	
	void			Draw	(  LPDIRECT3DDEVICE9 device );
	bool			Update	( int posXMouse, int posYMouse, bool isClicking );
	unsigned int	GetId	( void ) const { return mIdButton; }

private:

	TypeImage						mImageToDraw;
	std::vector<CAnimationImage>	mImages;
	unsigned int					mIdButton;
	
	int		mSoundClick;
	int		mSoundOverMouse;

	int		mPosX;
	int		mPosY;
	int		mHeight;
	int		mWidth;	
};

#endif //INC_BUTTON_H_