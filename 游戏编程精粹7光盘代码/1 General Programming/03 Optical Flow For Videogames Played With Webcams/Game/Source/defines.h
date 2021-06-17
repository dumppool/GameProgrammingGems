#ifndef INC_DEFINES_H
#define INC_DEFINES_H

#include <string>
#include <d3dx9.h>

#define KEYDOWN(name, key) (name[key] & 0x80) 
#define PATH_TEXTURES		"..\\Data\\Textures\\"

enum Button_ID {	ePlayButton, eExitButton, eCreditButton, eAboutButton, eConfigButton, eMenuButton, 
					eEasyLevelButton, eMediumLevelButton, eDifficultLevelButton, ePauseButton};

enum Window_ID {eMainWindow, ePlayWindow, eGameOverWindow, eCreditWindow, eAboutWindow, eConfigWindow };

struct SCREEN_COLOR_VERTEX
{
	float x, y, z, w;
	DWORD color;
    float u, v; 
	static unsigned int getFlags()
	{
		return(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	}
};

struct SCREEN_VERTEX
{
	float x,y,z,w;
	float u,v;
	static unsigned int getFlags()
	{
		return(D3DFVF_XYZRHW|D3DFVF_TEX1);
	}
};

#define CHECKED_DELETE(x) if(x!=NULL) {delete x; x=NULL;}
#define CHECKED_RELEASE(x) if(x!=NULL) {x->Release(); x=NULL;}


#endif //INC_DEFINES_H