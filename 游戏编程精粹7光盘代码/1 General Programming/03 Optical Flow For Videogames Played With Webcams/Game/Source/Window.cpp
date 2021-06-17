#include "Window.h"
#include "KeyboardInput.h"

CWindow::CWindow( const CAnimationImage & newImage, const std::vector<CButton>& buttons, unsigned int id)
: mButtons( buttons )
, mImage( newImage )
, mIsClicked( false )
, mIdWindow(id)
{}

void CWindow::Draw( LPDIRECT3DDEVICE9 device  )
{
	mImage.Draw(device);
	std::vector<CButton>::iterator it;
	for( it = mButtons.begin(); it != mButtons.end(); ++it )
	{
		(*it).Draw(device);
	}
	
}
	
void CWindow::Update(	float deltaTime, int posXMouse, int posYMouse,  
						bool isClicking )
{
	std::vector<CButton>::iterator it;
	for( it = mButtons.begin(); it != mButtons.end(); ++it )
	{
		if( (*it).Update( posXMouse, posYMouse, isClicking) )
		{
			mButtonClicked = (*it).GetId();
			mIsClicked = true;
			break;
		}
		else
		{
			mIsClicked = false;
		}
	}
	mImage.Update(deltaTime);
}