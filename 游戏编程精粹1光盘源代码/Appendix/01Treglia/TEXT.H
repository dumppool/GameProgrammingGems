/* Copyright (C) Dante Treglia II, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Dante Treglia II, 2000"
 */
//===========================================================================
//===========================================================================
#ifndef _TEXT_H
#define _TEXT_H
#include <cstdio>
#include <cstdarg>
#ifdef WIN32
#include "win32.h"
#endif
#include "def.h"

#define MAX_STR 4096

#define TEXT_NONE 0x00
#define TEXT_DRAW_BOX 0x04
#define TEXT_DRAW_SPACES 0x02
#define TEXT_WRAP_SPACES 0x08
#define TEXT_DRAW_BACKGROUND 0x01

class TextBox {
public:
	TextBox(int boxL, int boxR, int boxT, int boxB);
	//~TextBox() {free(buffer);}

private:
	// Bounding Box
	int boxL;
	int boxR;
	int boxT;
	int boxB;

	// Text Buffers
	char * buffer;
	char * drawBufferPtr;

	// Info
	int pageLineCnt;
	int scrollCnt;
	int currX;
	int currY;
	COLOR color;

public:
	unsigned int mode;

public:
	void Begin();
	void End();
	void DrawStr(int x, int y, char* string);
	void Draw();
	void Printf(char* fmt, ...);
	void Clear() {buffer[0] = '\0';};
	void SetColor(COLOR colorIn) {color = colorIn;};
	void SetMode(unsigned int modeIn) {mode = modeIn;};
	void PageUp() {scrollCnt += pageLineCnt;};
	void PageDown() {scrollCnt -= pageLineCnt; if (scrollCnt < 0) scrollCnt = 0;};
	void PageReset() {scrollCnt = 0;};

private:
	void Paginate();	
	void DrawStr(int x, int y, int maxFlag, int maxY, char* string);
	void DrawBoundingBox();
	void FormatStrCat(char * str);
};

#endif 
