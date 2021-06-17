#include <dinput.h>

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


class CKeyboardInput
{

public:
	
	CKeyboardInput();				
	~CKeyboardInput();
	//--- Crea el device del Keyboard mediante DirectInput
	HRESULT		OnCreateDevice		( HWND hWnd );
	
	//--- Obtiene toda la informacion del teclado (rellena la info. del m_diks)
	HRESULT		ReadImmediateData	();

	//--- Elimina la memoria alocatada
	void		FreeDirectInput		( void );

	//--- Obtiene la estructura m_diks (que contiene que teclas estan presionadas)
	char *		GetKey				( void );

private:
	
	char					m_diks[256];	// DirectInput keyboard state buffer 
	LPDIRECTINPUT8			m_pDI;			// The DirectInput object         
	LPDIRECTINPUTDEVICE8	m_pKeyboard;	// The keyboard device 
};

#endif //INC_KEYBOARD_H_