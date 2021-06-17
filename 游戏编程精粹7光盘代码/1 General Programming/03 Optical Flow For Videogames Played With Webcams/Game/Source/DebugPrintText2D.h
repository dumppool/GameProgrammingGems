// Esta clase nos permite pintar en la pantall texto 2D. La cual cosa puede ser
// muy util para debugar, es decir, visualizar valores de ciertas variables como 
// por ejemplo la posicion del player, el numero de rooms que se estan pintando, etc.

#ifndef __DEBUGPRINT_H__
#define __DEBUGPRINT_H__

#include <d3dx9.h>

typedef enum {TIMES_NEW_ROMAN, SNAPITC, SNAPITC_BIG }TYPE_FONT;

class CDebugPrintText2D
{
public:
	
	CDebugPrintText2D();
	~CDebugPrintText2D();

	// Funcion que inicializa la variable m_font, definiendo el tipo de letra a usar
	void	SetFonts	( LPDIRECT3DDEVICE9 D3Ddevice );
	

	// Funcion que hemos de llamar cada vez que queramos pintar un texto.
	int		PrintText	( TYPE_FONT font, int x, int y, DWORD color, const char * format, ... );

private:

	LPD3DXFONT	m_font_TimesNewRoman;	
	LPD3DXFONT	m_font_SnapITC;	
	LPD3DXFONT	m_font_SnapITC_BIG;
};

#endif //__DEBUGPRINT_H__