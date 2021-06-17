#ifndef _APPLICATION_DX_H
#define _APPLICATION_DX_H

#include <d3dx9.h>
#include "Timer.h"
#include "DebugPrinttext2D.h"
#include "WebCamera.h"
#include "Game.h"

class CApplicationDX
{

public:

	CApplicationDX	(void);
	~CApplicationDX	(void);

	//--- Iniciliza la aplicacion y carga en memoria los datos
	void		InitAndLoad	( HWND hWnd );
	
	//--- Lee los inputs de entrada (Mouse y Keyboard) y actualiza todos los elementos del juego
	bool		Update		( void );

	//--- Dibuja en pantalla todos los elementos que deben ser renderizados
	void		Render		( void );


private:

	CGame	game;
	
	LPDIRECT3D9             m_pD3D;
	LPDIRECT3DDEVICE9       m_pD3DDevice;
	
	CTimer		m_timer;			//Objeto con toda la informacion del tiempo (fps, ElapsedTime, etc)
	HWND		m_hWnd;				//Identificador de ventana donde se incrusta el visualizador 

	

private:

	//La funcion Init()  llama a InitDX() y InitData()
	//--- InitDX inicializa todo lo relacionado con DirectX
	HRESULT		InitDX			( HWND hWnd );
	
	
	//--- RENDER FUNCTIONS ---
	//La funcion Render() llama a estas funciones
	//Prepara la visualizacion (obteniendo la matrix View y Projection)
	void		SetupMatrices	( void );
	//Prepara las variables de Direct3D para hacer el render de la escena
	void		BeginRenderDX	( void );
	//Finaliza la composicion de la escena con Direct3D
	void		EndRenderDX		( void );
};

#endif //_APPLICATION_DX_H