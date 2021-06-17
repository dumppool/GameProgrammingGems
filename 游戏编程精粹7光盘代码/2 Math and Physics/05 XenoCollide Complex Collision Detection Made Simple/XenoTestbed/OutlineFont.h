
#pragma once

#include <string>
#include <gl/gl.h>

//////////////////////////////////////////////////////////////////////////////
// OutlineFont is a helper class that supports drawing text using any
// Windows outline (i.e., non-bitmap) font.

class OutlineFont
{

public:

    OutlineFont( HDC hdc, const char* fontname );
    virtual ~OutlineFont();
	
    void DrawString( const char* str );

private:

	GLuint m_listbase;
    HDC m_hDC;

};
