#ifndef INC_POINTERMOUSE_H_
#define INC_POINTERMOUSE_H_

#include <vector>
#include "AnimationImage.h"

class CPointerMouse
{

public:

	CPointerMouse( );
	
	void		SetImages	(  const std::vector<LPDIRECT3DTEXTURE9> & textures, int h, int w );
	void		Draw		( LPDIRECT3DDEVICE9 device, const float& alpha = 1 );
	void		Update		( float deltaTime, float incX, float incY );
	int			GetX		( void ) const {return (int)mPosX;}
	int			GetY		( void ) const {return (int)mPosY;}
	int			GetCenterX	( void ) const {return (int) (mPosX + mW*0.5);}
	int			GetCenterY	( void ) const {return (int) (mPosY + mH*0.5);}


private:

	std::vector<CAnimationImage>	mImages;

	float	mPosX;
	float	mPosY;
	int		mW;
	int		mH;
	int		mImageToDraw;
};

#endif //INC_POINTERMOUSE_H_