#include "DebugPrintText2D.h"
#include "Defines.h"

#pragma warning( disable : 4996 ) // disable deprecated warning 

CDebugPrintText2D::CDebugPrintText2D()
: m_font_TimesNewRoman(NULL)
, m_font_SnapITC(NULL)
, m_font_SnapITC_BIG(NULL)
{}

CDebugPrintText2D::~CDebugPrintText2D()
{
	// Si hemos inicializado la variable m_font, debemos de eliminar la memoria dinamica
	if(m_font_TimesNewRoman != NULL)
	{
		m_font_TimesNewRoman->Release();
		m_font_TimesNewRoman = NULL;
	}
	if(m_font_SnapITC != NULL)
	{
		m_font_SnapITC->Release();
		m_font_SnapITC = NULL;
	}
	
	if(m_font_SnapITC_BIG != NULL)
	{
		m_font_SnapITC_BIG->Release();
		m_font_SnapITC_BIG = NULL;
	}

}


 /*Esta funcion define el tipo de letra a usar.
	LPDIRECT3DDEVICE9 D3Ddevice -> device3D que necesitamos para crear la fuente de texto*/
void CDebugPrintText2D::SetFonts(LPDIRECT3DDEVICE9 D3Ddevice)
{
	D3DXCreateFont( D3Ddevice, 17, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,				
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,	
				"Times New Roman",	&m_font_TimesNewRoman);	
	D3DXCreateFont( D3Ddevice, 40, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,				
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,	
				"Snap ITC",	&m_font_SnapITC);	
	D3DXCreateFont( D3Ddevice, 80, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,				
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,	
				"Snap ITC",	&m_font_SnapITC_BIG);
}

// Funcion principal de esta clase, con la cual podemos utilizar la variable m_font
// para pintar texto2D en pantalla. Los parametros de entrada son:
//	- int x -> posicion en la coordenada X donde pintaremos el texto.
//	- int y -> posicion en la coordenada Y donde pintaremos el texto.
//	- DWORD color -> color expresado en en rgba (red, green, blue y alpha) 
//	- const char * format -> texto a escribir con posibles tags para expresar valores de 
//	  variables, es decir, %s %d %f etc.
//	- ... -> la rista de posibles variables a las que hace referencia los tags %
int CDebugPrintText2D::PrintText(TYPE_FONT font, int x, int y, DWORD color, const char * format, ... )
{
	va_list args;
	char* buffer;

	va_start(args,format);
 	int len = _vscprintf(format, args) + 1;

	buffer = (char*)malloc(len*sizeof(char));
	vsprintf( buffer, format, args );

	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.bottom = rect.right = 0;

	
	// Calcular el tamaño del rectangulo necesario para pintar ese texto y pintarlo
	switch(font)
	{
	case TIMES_NEW_ROMAN:
		m_font_TimesNewRoman->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color );
		m_font_TimesNewRoman->DrawText( NULL, buffer, -1, &rect, DT_LEFT, color );
		break;
	case SNAPITC:
		m_font_SnapITC->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color );
		m_font_SnapITC->DrawText( NULL, buffer, -1, &rect, DT_LEFT, color );
		break;
	case SNAPITC_BIG:
		m_font_SnapITC_BIG->DrawText( NULL, buffer, -1, &rect, DT_CALCRECT, color );
		m_font_SnapITC_BIG->DrawText( NULL, buffer, -1, &rect, DT_LEFT, color );
		break;
	}

	
	free( buffer );

	return (rect.bottom  - rect.top);
}
