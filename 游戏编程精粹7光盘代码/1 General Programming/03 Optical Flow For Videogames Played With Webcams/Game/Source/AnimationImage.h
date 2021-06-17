#ifndef INC_IMAGE_H_
#define INC_IMAGE_H_

#include <d3dx9.h>
#include <vector>
#include "defines.h"



class CAnimationImage
{

public:

	CAnimationImage(	const std::vector<LPDIRECT3DTEXTURE9> & textures, float animationTime, int posX,
			int posY, int height, int width );
	
	CAnimationImage( LPDIRECT3DTEXTURE9 texture, int posX, int posY, int h, int w );
	
	void		Draw	( LPDIRECT3DDEVICE9 device, const float& alpha = 1 );
	void		Update	( float deltaTime );

	void		SetPos				( int newPosX, int newPosY ) {mPosX = newPosX; mPosY = newPosY; }
	void		SetWidthAndHeight	(int w, int h) {mWidth = w; mHeight = h;}

private:

	std::vector<LPDIRECT3DTEXTURE9>					mTextures;
	std::vector<LPDIRECT3DTEXTURE9>::const_iterator	mItCurrentTexture;
	
	float	mAnimationTime;
	float	mTime;

	int		mPosX;
	int		mPosY;
	int		mHeight;
	int		mWidth;	
	

};

#endif //INC_IMAGE_H_