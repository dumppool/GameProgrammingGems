#include "WindowSettings.h"


CWindowSettings *CWindowSettings::myWindowSettings = 0;


//--------CONSTRUCTOR & DESTRUCTOR------------------//
CWindowSettings::CWindowSettings()
: mWidth( 0 )
, mHeight( 0 )
{}

CWindowSettings::~CWindowSettings()
{}
//----------------------------------------------//


//--------------SINGLETON FUNCTIONS------------------//
void CWindowSettings::CleanUP()
{
	if( myWindowSettings != 0 )
	{
		delete myWindowSettings;
		myWindowSettings = 0;
	}	
}

CWindowSettings *CWindowSettings::GetInstance()
{
	if( myWindowSettings == 0 )
	{
		myWindowSettings = new CWindowSettings;
	};

	return myWindowSettings;
}
//----------------------------------------------------//


