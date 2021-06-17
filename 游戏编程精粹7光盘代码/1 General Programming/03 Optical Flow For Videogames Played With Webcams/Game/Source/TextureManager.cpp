#include ".\texturemanager.h"
#include <assert.h>
#include "defines.h"

void GetFilename(const char *path, std::string *filename)
{
	char *pnt=NULL;
	for(int b=(int)strlen(path);b>0;--b)
	{
		if(path[b]=='\\')
		{
			pnt=(char*)&(path[b+1]);
			b=-1;
		}
	}
	if(pnt==NULL)
		pnt=(char*)&(path[0]);
	*filename=pnt;
}

CTextureManager * CTextureManager::m_TextureManager=NULL;

CTextureManager * CTextureManager::GetTextureManager()
{
	if(m_TextureManager==NULL)
		m_TextureManager=new CTextureManager;
	assert(m_TextureManager!=NULL);
	return m_TextureManager;
}

CTextureManager::CTextureManager()
{
	
}

CTextureManager::~CTextureManager()
{
}

void CTextureManager::CleanUp()
{
	for(TTextureIterator it=m_TextureMap.begin();it!=m_TextureMap.end();++it)
	{
		CHECKED_RELEASE(it->second);
	}
	m_TextureMap.clear();
	CHECKED_DELETE(m_TextureManager);
}

//
//LPDIRECT3DTEXTURE9 CTextureManager::LoadTextureWebCam( LPDIRECT3DDEVICE9 Device )
//{
//	HRESULT D3DXCreateTextureFromFileEx( Device,
//				  LPCTSTR pSrcFile,
//				  UINT Width,
//				  UINT Height,
//				  UINT MipLevels,
//				  DWORD Usage,
//				  D3DFORMAT Format,
//				  D3DPOOL Pool,
//				  DWORD Filter,
//				  DWORD MipFilter,
//				  D3DCOLOR ColorKey,
//				  D3DXIMAGE_INFO * pSrcInfo,
//				  PALETTEENTRY * pPalette,
//				  LPDIRECT3DTEXTURE9 * ppTexture
//				);
//}
LPDIRECT3DTEXTURE9 CTextureManager::LoadTexture(const std::string &filename, LPDIRECT3DDEVICE9 Device)
{
	LPDIRECT3DTEXTURE9 l_Texture=NULL;
	if(m_TextureMap.find(filename)!=m_TextureMap.end())
		l_Texture=m_TextureMap[filename];
	else
	{
		std::string l_FilenameWithoutPath, l_FilenameComplete;
		GetFilename(filename.c_str(),&l_FilenameWithoutPath);

		l_FilenameComplete=PATH_TEXTURES;
		l_FilenameComplete+=l_FilenameWithoutPath;
		D3DXCreateTextureFromFile(Device,l_FilenameComplete.c_str(),&l_Texture);
		m_TextureMap[filename]=l_Texture;
	}
	assert(l_Texture!=NULL);
	return l_Texture;
}
