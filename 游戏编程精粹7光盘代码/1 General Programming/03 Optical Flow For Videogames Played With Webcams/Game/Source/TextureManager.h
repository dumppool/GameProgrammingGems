#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <d3dx9.h>
#include <map>
#include <string>

class CTextureManager
{

public:
	CTextureManager();
	~CTextureManager();

	static CTextureManager * GetTextureManager();
	
	void				CleanUp				( void );
	LPDIRECT3DTEXTURE9	LoadTexture			( const std::string &filename, LPDIRECT3DDEVICE9 Device );
	LPDIRECT3DTEXTURE9	LoadTextureWebCam	( LPDIRECT3DDEVICE9 Device );

private:

	typedef std::map<std::string,LPDIRECT3DTEXTURE9>::iterator TTextureIterator;

	static CTextureManager *					m_TextureManager;
	std::map<std::string,LPDIRECT3DTEXTURE9>	m_TextureMap;

};

#endif // TEXTURE_MANAGER_H