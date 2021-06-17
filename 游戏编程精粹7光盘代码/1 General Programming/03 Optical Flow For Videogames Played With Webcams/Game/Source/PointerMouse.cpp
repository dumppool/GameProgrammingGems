#include "PointerMouse.h"
#include "WindowSettings.h"

CPointerMouse::CPointerMouse( )
: mPosX( 0 )
, mPosY( 0 )
, mW(0)
, mH(0)
, mImageToDraw( -1 )
{}

void CPointerMouse::Draw( LPDIRECT3DDEVICE9 device, const float& alpha )
{
	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE  );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA  );
	device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	if( mImageToDraw != -1 )
		mImages[mImageToDraw].Draw( device, alpha );

	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_TRUE  );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, FALSE );
}

void CPointerMouse::Update( float deltaTime, float incX, float incY )
{
	mPosX += incX*deltaTime*50;
	mPosY += incY*deltaTime*50;
	//mPosX += incX*0.1;
	if( mPosX  > 100.f - mW )
		mPosX = 100.f - mW;
	else if ( mPosX < 0.f )
		mPosX = 0.f;

	//mPosY += incY*0.1;
	if( mPosY > 100.f - mH  )
		mPosY = 100.f - mH ;
	else if( mPosY < 0.f )
		mPosY = 0.f;

	if( mImageToDraw != -1 )
	{
		mImages[mImageToDraw].SetPos( (int)mPosX, (int)mPosY );
		mImages[mImageToDraw].Update( deltaTime );
	}
}

void CPointerMouse::SetImages(  const std::vector<LPDIRECT3DTEXTURE9> & textures, int h, int w )
{
	mImageToDraw = 0;
	std::vector<LPDIRECT3DTEXTURE9>::const_iterator it;
	for(it = textures.begin(); it != textures.end(); ++it)
	{
		CAnimationImage image( (*it), 0, 0, h, w);
		mImages.push_back(image);
	}
	mW = w;
	mH = h;
}