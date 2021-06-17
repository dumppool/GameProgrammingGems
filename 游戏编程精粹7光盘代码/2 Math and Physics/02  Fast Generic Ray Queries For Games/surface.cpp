// -----------------------------------------------------------
// KdTreeDemo - surface.cpp
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#include "surface.h"

namespace KdTreeDemo {

// -----------------------------------------------------------
// Hicolor surface class implementation
// -----------------------------------------------------------

Surface::Surface( int a_Width, int a_Height, Pixel* a_Buffer, int a_Pitch ) :
	m_Width( a_Width ),
	m_Height( a_Height ),
	m_Buffer( a_Buffer ),
	m_Pitch( a_Pitch )
{
}

Surface::Surface( int a_Width, int a_Height ) :
	m_Width( a_Width ),
	m_Height( a_Height ),
	m_Pitch( a_Width )
{
	m_Buffer = new Pixel[a_Width * a_Height];
}

Surface::~Surface()
{
	delete [] m_Buffer;
}

void Surface::Clear( Pixel a_Color )
{
	Pixel* a = m_Buffer;
	for ( int i = 0; i < m_Height; i++ ) 
	{
		for ( int x = 0; x < m_Width; x++ ) a[x] = a_Color;
		a += m_Pitch;
	}
}

void Surface::CopyTo( Surface* a_Dst, int a_X, int a_Y )
{
	Pixel* dst = a_Dst->GetBuffer();
	Pixel* src = m_Buffer;
	if ((src) && (dst)) 
	{
		int srcwidth = m_Width;
		int srcheight = m_Height;
		int srcpitch = m_Pitch;
		int dstwidth = a_Dst->GetWidth();
		int dstheight = a_Dst->GetHeight();
		int dstpitch = a_Dst->GetPitch();
		if ((srcwidth + a_X) > dstwidth) srcwidth = dstwidth - a_X;
		if ((srcheight + a_Y) > dstheight) srcheight = dstheight - a_Y;
		if (a_X < 0) src -= a_X, srcwidth += a_X, a_X =0;
		if (a_Y < 0) src -= a_Y * srcpitch, srcheight += a_Y, a_Y = 0;
		if ((srcwidth > 0) && (srcheight > 0))
		{
			dst += a_X + dstpitch * a_Y;
			for ( int y = 0; y < srcheight; y++ )
			{
				memcpy( dst, src, srcwidth * 4 );
				dst += dstpitch;
				src += srcpitch;
			}
		}
	}
}

void Surface::Print( char* a_String, int x1, int y1, Pixel color )
{
	Pixel* t = m_Buffer + x1 + y1 * m_Pitch;
	int i;
	for ( i = 0; i < (int)(strlen( a_String )); i++ )
	{	
		long pos = 0;
		if ((a_String[i] >= 'A') && (a_String[i] <= 'Z')) pos = s_Transl[(unsigned short)(a_String[i] - ('A' - 'a'))];
													 else pos = s_Transl[(unsigned short)a_String[i]];
		Pixel* a = t;
		char* c = (char*)s_Font[pos];
		int h, v;
		for ( v = 0; v < 5; v++ ) 
		{
			for ( h = 0; h < 5; h++ ) if (*c++ == 'o') *(a + h) = color;
			a += m_Pitch;
		}
		t += 6;
	}
}

void Surface::Line( float x1, float y1, float x2, float y2, Pixel c )
{
	if ((x1 < 0) || (y1 < 0) || (x1 >= m_Width) || (y1 >= m_Height) ||
		(x2 < 0) || (y2 < 0) || (x2 >= m_Width) || (y2 >= m_Height))
	{
		return;
	}
	float b = x2 - x1;
	float h = y2 - y1;
	float l = fabsf( b );
	if (fabsf ( h ) > l) l = fabsf( h );
	int il = (int)l;
	float dx = b / (float)l;
	float dy = h / (float)l;
	for ( int i = 0; i <= il; i++ )
	{
		*(m_Buffer + (int)x1 + (int)y1 * m_Pitch) = c;
		x1 += dx, y1 += dy;
	}
}

void Surface::Plot( int x, int y, Pixel c )
{
	if ((x >= 0) && (y >= 0) && (x < SCRWIDTH) && (y < SCRHEIGHT))
	{
		m_Buffer[x + y * SCRWIDTH] = c;
	}
}

void Surface::SetChar( int c, char* c1, char* c2, char* c3, char* c4, char* c5 )
{
	strcpy( s_Font[c][0], c1 );
	strcpy( s_Font[c][1], c2 );
	strcpy( s_Font[c][2], c3 );
	strcpy( s_Font[c][3], c4 );
	strcpy( s_Font[c][4], c5 );
}

void Surface::InitCharset()
{
	SetChar( 0, ":ooo:", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 1, "oooo:", "o:::o", "oooo:", "o:::o", "oooo:" );
	SetChar( 2, ":oooo", "o::::", "o::::", "o::::", ":oooo" );
	SetChar( 3, "oooo:", "o:::o", "o:::o", "o:::o", "oooo:" );
	SetChar( 4, "ooooo", "o::::", "oooo:", "o::::", "ooooo" );
	SetChar( 5, "ooooo", "o::::", "ooo::", "o::::", "o::::" );
	SetChar( 6, ":oooo", "o::::", "o:ooo", "o:::o", ":ooo:" );
	SetChar( 7, "o:::o", "o:::o", "ooooo", "o:::o", "o:::o" );
	SetChar( 8, "::o::", "::o::", "::o::", "::o::", "::o::" );
	SetChar( 9, ":::o:", ":::o:", ":::o:", ":::o:", "ooo::" );
	SetChar(10, "o::o:", "o:o::", "oo:::", "o:o::", "o::o:" );
	SetChar(11, "o::::", "o::::", "o::::", "o::::", "ooooo" );
	SetChar(12, "oo:o:", "o:o:o", "o:o:o", "o:::o", "o:::o" );
	SetChar(13, "o:::o", "oo::o", "o:o:o", "o::oo", "o:::o" );
	SetChar(14, ":ooo:", "o:::o", "o:::o", "o:::o", ":ooo:" );
	SetChar(15, "oooo:", "o:::o", "oooo:", "o::::", "o::::" );
	SetChar(16, ":ooo:", "o:::o", "o:::o", "o::oo", ":oooo" );
	SetChar(17, "oooo:", "o:::o", "oooo:", "o:o::", "o::o:" );
	SetChar(18, ":oooo", "o::::", ":ooo:", "::::o", "oooo:" );
	SetChar(19, "ooooo", "::o::", "::o::", "::o::", "::o::" );
	SetChar(20, "o:::o", "o:::o", "o:::o", "o:::o", ":oooo" );
	SetChar(21, "o:::o", "o:::o", ":o:o:", ":o:o:", "::o::" );
	SetChar(22, "o:::o", "o:::o", "o:o:o", "o:o:o", ":o:o:" );
	SetChar(23, "o:::o", ":o:o:", "::o::", ":o:o:", "o:::o" );
	SetChar(24, "o:::o", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar(25, "ooooo", ":::o:", "::o::", ":o:::", "ooooo" );
	SetChar(26, ":ooo:", "o::oo", "o:o:o", "oo::o", ":ooo:" );
	SetChar(27, "::o::", ":oo::", "::o::", "::o::", ":ooo:" );
	SetChar(28, ":ooo:", "o:::o", "::oo:", ":o:::", "ooooo" );
	SetChar(29, "oooo:", "::::o", "::oo:", "::::o", "oooo:" );
	SetChar(30, "o::::", "o::o:", "ooooo", ":::o:", ":::o:" );
	SetChar(31, "ooooo", "o::::", "oooo:", "::::o", "oooo:" );
	SetChar(32, ":oooo", "o::::", "oooo:", "o:::o", ":ooo:" );
	SetChar(33, "ooooo", "::::o", ":::o:", "::o::", "::o::" );
	SetChar(34, ":ooo:", "o:::o", ":ooo:", "o:::o", ":ooo:" );
	SetChar(35, ":ooo:", "o:::o", ":oooo", "::::o", ":ooo:" );
	SetChar(36, "::o::", "::o::", "::o::", ":::::", "::o::" );
	SetChar(37, ":ooo:", "::::o", ":::o:", ":::::", "::o::" );
	SetChar(38, ":::::", ":::::", "::o::", ":::::", "::o::" );
	SetChar(39, ":::::", ":::::", ":ooo:", ":::::", ":ooo:" );
	SetChar(40, ":::::", ":::::", ":::::", ":::o:", "::o::" );
	SetChar(41, ":::::", ":::::", ":::::", ":::::", "::o::" );
	SetChar(42, ":::::", ":::::", ":ooo:", ":::::", ":::::" );
	SetChar(43, ":::o:", "::o::", "::o::", "::o::", ":::o:" );
	SetChar(44, "::o::", ":::o:", ":::o:", ":::o:", "::o::" );
	SetChar(45, ":::::", ":::::", ":::::", ":::::", ":::::" );
	SetChar(46, "ooooo", "ooooo", "ooooo", "ooooo", "ooooo" );
	SetChar(47, "::o::", "::o::", ":::::", ":::::", ":::::" ); // Tnx Ferry
	SetChar(48, "o:o:o", ":ooo:", "ooooo", ":ooo:", "o:o:o" );
	SetChar(49, "::::o", ":::o:", "::o::", ":o:::", "o::::" );
	char c[] = "abcdefghijklmnopqrstuvwxyz0123456789!?:=,.-() #'*/";
	int i;
	for ( i = 0; i < 256; i++ ) s_Transl[i] = 45;
	for ( i = 0; i < 50; i++ ) s_Transl[(unsigned char)c[i]] = i;
}

}; // namespace KdTreeDemo
