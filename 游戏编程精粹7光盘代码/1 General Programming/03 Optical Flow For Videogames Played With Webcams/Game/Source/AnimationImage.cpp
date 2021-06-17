#include "AnimationImage.h"

#include <assert.h>
#include "WindowSettings.h"


CAnimationImage::CAnimationImage( const std::vector<LPDIRECT3DTEXTURE9> & textures, float animationTime, int posX,
				int posY, int h, int w )
: mTextures( textures )
, mAnimationTime( animationTime )
, mItCurrentTexture( mTextures.begin() )
, mTime (0.f)
, mPosX( posX )
, mPosY( posY )
, mHeight( h )
, mWidth( w )
{}
	
CAnimationImage::CAnimationImage( LPDIRECT3DTEXTURE9 texture,int posX, int posY, int h, int w  )
: mAnimationTime( 0.f )
, mTime (0.f)
, mPosX( posX )
, mPosY( posY )
, mHeight( h )
, mWidth( w)
{
	mTextures.push_back( texture );
	mItCurrentTexture = mTextures.begin();
}
	

void CAnimationImage::Draw( LPDIRECT3DDEVICE9 device, const float& alpha )
{
	if( alpha > 0 )
	{
		int w = CWindowSettings::GetInstance()->GetWidth();
		int h = CWindowSettings::GetInstance()->GetHeight();

		// render el quad de difuminacion....
		device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); 
		device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); 

		unsigned short l_Indexes[6]={0,1,2,1,3,2};
		/*
		A(x1,y1)	mPos /------------------\	C(x2,y1)
				   	     |              /   |
					     |		     /      |
					     |      / *         |
				 	     | /                |
		B(x1,y2)	     \------------------/	D(x2,y2)*/
	
		float x1 = mPosX * 0.01f * w;
		float x2 = (mPosX + mWidth)*0.01f * w;
		float y1 = mPosY *0.01f * h;
		float y2 = (mPosY + mHeight)*0.01f * h;

		D3DXVECTOR3 PointA( x1, y2, 0.f );
		D3DXVECTOR3 PointB( x1, y1, 0.f );
		D3DXVECTOR3 PointC( x2, y2, 0.f );
		D3DXVECTOR3 PointD( x2, y1, 0.f );

		
		SCREEN_COLOR_VERTEX v[4] = {
				{ PointA.x, PointA.y, PointA.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 0,1}//(x,y) sup_esq.
			,	{ PointB.x, PointB.y, PointB.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 0,0}//(x,y) inf_esq.
			,   { PointC.x, PointC.y, PointC.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 1,1}//(x,y) sup_dr.
			,	{ PointD.x, PointD.y, PointD.z, 1, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, alpha ), 1,0}//(x,y) inf_dr.
		};		
		device->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
		device->SetTexture(0,mTextures[0]);
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0, 4, 2, l_Indexes, D3DFMT_INDEX16,v,sizeof(SCREEN_COLOR_VERTEX));
	}
}
	
void CAnimationImage::Update	( float deltaTime )
{
	assert( mTextures.size() );

	if( mTextures.size() != 1 )
	{
		mTime += deltaTime;
		if( mTime > mAnimationTime )
		{
			mTime = 0.f;
			mItCurrentTexture++;
			if (mItCurrentTexture == mTextures.end() )
				mItCurrentTexture = mTextures.begin();
		}
	}	
}


	