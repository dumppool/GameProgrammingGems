#ifndef _DEBUGRENDER_H
#define _DEBUGRENDER_H

#include <d3dx9.h>

class CDebugRender
{
public:
	static void DrawAxis(LPDIRECT3DDEVICE9 Device, float Size);
	static void DrawGrid(LPDIRECT3DDEVICE9 Device, float Size, DWORD Color=0xffffffff, int GridX=10, int GridZ=10);
	static void DrawCube(LPDIRECT3DDEVICE9 Device, float Size, DWORD Color=0xffffffff);
	static void DrawSphere(LPDIRECT3DDEVICE9 Device, float Radius, DWORD Color=0xffffffff, int Aristas=10);
	static void DrawLine(LPDIRECT3DDEVICE9 Device, const D3DXVECTOR3 &PosA, const D3DXVECTOR3 &PosB, DWORD Color=0xffffffff);
};

#endif