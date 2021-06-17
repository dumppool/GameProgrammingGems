// -----------------------------------------------------------
// KdTreeDemo - surface.h
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#ifndef I_SURFACE_H
#define I_SURFACE_H

#include "string.h"
#include "template.h"

namespace KdTreeDemo {

class Surface
{
public:
	// constructor / destructors
	Surface( int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch );
	Surface( int a_Width, int a_Height );
	~Surface();

	// member data access
    Pixel* GetBuffer() { return m_Buffer; }
    void SetBuffer( Pixel* a_Buffer ) { m_Buffer = a_Buffer; }
	int GetWidth() { return m_Width; }
	int GetHeight() { return m_Height; }
	int GetPitch() { return m_Pitch; }
	void SetPitch( int a_Pitch ) { m_Pitch = a_Pitch; }

	// Special operations
	void InitCharset();
	void SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 );
	void Print( char* a_String, int x1, int y1, Pixel color );
	void Clear( Pixel a_Color );
	void Line( float x1, float y1, float x2, float y2, Pixel color );
	void Plot( int x, int y, Pixel color );
	void CopyTo( Surface* a_Dst, int a_X, int a_Y );
	
private:
	// Attributes
	Pixel* m_Buffer;	
	int m_Width, m_Height, m_Pitch;	
	
	// Static attributes for the buildin font
	char s_Font[51][5][5];	
	int s_Transl[256];		
};

}; // namespace KdTreeDemo

#endif
