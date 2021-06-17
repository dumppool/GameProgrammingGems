#include <assert.h>
#include "Button.h"
#include "SoundManager.h"

CButton::CButton(	const std::vector<LPDIRECT3DTEXTURE9> & textures, int posX, 
					int posY, int h, int w, int soundClick, int soundOverMouse, unsigned int idButton )
: mIdButton( idButton )
, mSoundClick( soundClick )
, mSoundOverMouse( soundOverMouse )
, mPosX( posX )
, mPosY( posY )
, mHeight( h )
, mWidth( w )
, mImageToDraw( eNormal )
{
	assert(textures.size()==3);
	CAnimationImage imageN(textures[0],  posX, posY, h, w);
	CAnimationImage imageM(textures[1],  posX, posY, h, w);
	CAnimationImage imageC(textures[2],  posX, posY, h, w);
	mImages.push_back(imageN);
	mImages.push_back(imageM);
	mImages.push_back(imageC);
}

void CButton::Draw( LPDIRECT3DDEVICE9 device )
{
	assert(mImages.size()==3);

	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE  );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA  );
	device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	mImages[mImageToDraw].Draw( device );

	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_TRUE  );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
}

bool CButton::Update( int posXMouse, int posYMouse, bool isClicking )
{
	bool isInside = false;
	bool isClicked = false;

	if( (posXMouse > mPosX) && (posXMouse < mPosX + mWidth) &&
		(posYMouse > mPosY) && (posYMouse < mPosY + mHeight) )
	{
		//PointerMouse is inside
		isInside = true;

		if( isClicking )
		{
			mImageToDraw = eClicked;
		}
		else
		{
			if( mImageToDraw == eClicked )
			{
				isClicked = true;
				SoundManager::getInstance().PlaySource("buttonClick");
			}
			mImageToDraw = eMouseOver;
		}
	}
	else
	{
		mImageToDraw = eNormal;
	}

	return isClicked;
}
