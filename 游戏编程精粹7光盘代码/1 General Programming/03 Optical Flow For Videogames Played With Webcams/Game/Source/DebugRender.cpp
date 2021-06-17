#include ".\debugrender.h"
#include "defines.h"
#include <assert.h>

void CDebugRender::DrawAxis(LPDIRECT3DDEVICE9 Device, float Size)
{
	DrawLine(Device,D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(Size,0.0f,0.0f),0xffff0000);
	DrawLine(Device,D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,Size,0.0f),0xff00ff00);
	DrawLine(Device,D3DXVECTOR3(0.0f,0.0f,0.0f),D3DXVECTOR3(0.0f,0.0f,Size),0xff0000ff);
}

void CDebugRender::DrawGrid(LPDIRECT3DDEVICE9 Device, float Size, DWORD Color, int GridX, int GridZ)
{
	assert(GridX>0);
	assert(GridZ>0);
	//LINEAS EN Z
	for(int b=0;b<=GridX;++b)
	{
		DrawLine(Device,D3DXVECTOR3(-Size/2.0f+((float)(b*Size))/(float)GridX,0.0f,-Size/2.0f),D3DXVECTOR3(-Size/2.0f+((float)(b*Size))/(float)GridX,0.0f,Size/2.0f));
	}
	//LINEAS EN X
	for(int b=0;b<=GridX;++b)
	{
		DrawLine(Device,D3DXVECTOR3(-Size/2.0f,0.0f,-Size/2.0f+((float)(b*Size))/(float)GridX),D3DXVECTOR3(Size/2.0f,0.0f,-Size/2.0f+((float)(b*Size))/(float)GridX));
	}
}

void CDebugRender::DrawCube(LPDIRECT3DDEVICE9 Device, float Size, DWORD Color)
{
	//suelo
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,-Size/2.0f,-Size/2.0f),D3DXVECTOR3(Size/2.0f,-Size/2.0f,-Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,-Size/2.0f,Size/2.0f),D3DXVECTOR3(Size/2.0f,-Size/2.0f,Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,-Size/2.0f,-Size/2.0f),D3DXVECTOR3(-Size/2.0f,-Size/2.0f,Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(Size/2.0f,-Size/2.0f,-Size/2.0f),D3DXVECTOR3(Size/2.0f,-Size/2.0f,Size/2.0f),Color);
	//tapa
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,Size/2.0f,-Size/2.0f),D3DXVECTOR3(Size/2.0f,Size/2.0f,-Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,Size/2.0f,Size/2.0f),D3DXVECTOR3(Size/2.0f,Size/2.0f,Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,Size/2.0f,-Size/2.0f),D3DXVECTOR3(-Size/2.0f,Size/2.0f,Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(Size/2.0f,Size/2.0f,-Size/2.0f),D3DXVECTOR3(Size/2.0f,Size/2.0f,Size/2.0f),Color);
	//lineas laterales
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,-Size/2.0f,-Size/2.0f),D3DXVECTOR3(-Size/2.0f,Size/2.0f,-Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(-Size/2.0f,-Size/2.0f,Size/2.0f),D3DXVECTOR3(-Size/2.0f,Size/2.0f,Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(Size/2.0f,-Size/2.0f,-Size/2.0f),D3DXVECTOR3(Size/2.0f,Size/2.0f,-Size/2.0f),Color);
	DrawLine(Device,D3DXVECTOR3(Size/2.0f,-Size/2.0f,Size/2.0f),D3DXVECTOR3(Size/2.0f,Size/2.0f,Size/2.0f),Color);

}

void CDebugRender::DrawSphere(LPDIRECT3DDEVICE9 Device, float Radius, DWORD Color, int Aristas)
{
	for(int t=0;t<Aristas;++t)
	{
		float l_RadiusRing=Radius*sin(DEG2RAD(180.0f*((float)t))/((float)Aristas));
		for(int b=0;b<Aristas;++b)
		{
			D3DXVECTOR3 l_PosA(l_RadiusRing*cos(DEG2RAD((float)(360.0f*(float)b)/((float)Aristas))),Radius*cos(DEG2RAD(180.0f*((float)t))/((float)Aristas)),l_RadiusRing*sin(DEG2RAD((float)(360.0f*(float)b)/((float)Aristas))));
			D3DXVECTOR3 l_PosB(l_RadiusRing*cos(DEG2RAD((float)(360.0f*(float)(b+1))/((float)Aristas))),Radius*cos(DEG2RAD(180.0f*((float)t))/((float)Aristas)),l_RadiusRing*sin(DEG2RAD((float)(360.0f*(float)(b+1))/((float)Aristas))));
			DrawLine(Device,l_PosA,l_PosB,Color);
			
			float l_RadiusNextRing=Radius*sin(DEG2RAD(180.0f*((float)(t+1)))/((float)Aristas));
			
			D3DXVECTOR3 l_PosC(l_RadiusRing*cos(DEG2RAD((float)(360.0f*(float)b)/((float)Aristas))),Radius*cos(DEG2RAD(180.0f*((float)t))/((float)Aristas)),l_RadiusRing*sin(DEG2RAD((float)(360.0f*(float)b)/((float)Aristas))));
			D3DXVECTOR3 l_PosD(l_RadiusNextRing*cos(DEG2RAD((float)(360.0f*(float)b)/((float)Aristas))),Radius*cos(DEG2RAD(180.0f*((float)(t+1)))/((float)Aristas)),l_RadiusNextRing*sin(DEG2RAD((float)(360.0f*(float)b)/((float)Aristas))));
			DrawLine(Device,l_PosC,l_PosD,Color);
		}
	}
}

void CDebugRender::DrawLine(LPDIRECT3DDEVICE9 Device, const D3DXVECTOR3 &PosA, const D3DXVECTOR3 &PosB, DWORD Color)
{
	CUSTOMVERTEX v[2] =
	{
		{PosA.x, PosA.y, PosA.z, Color},
		{PosB.x, PosB.y, PosB.z, Color},
	};

	Device->SetTexture(0,NULL);
	Device->SetFVF(CUSTOMVERTEX::getFlags());
	Device->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}
