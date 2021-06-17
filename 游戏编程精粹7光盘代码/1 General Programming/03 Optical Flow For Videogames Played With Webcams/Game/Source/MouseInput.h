#ifndef INC_MOUSE_H_
#define INC_MOUSE_H_

#include <dinput.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


class CMouseInput
{

public:
	
	CMouseInput();				
	~CMouseInput();
	
	// Create & Destroy mouse input
	HRESULT		OnCreateDevice		( HWND hWnd );
	void		FreeDirectInput		( void );

	//--- READ MOUSE INPUT ---
	HRESULT		ReadImmediateData	( void );
	
	//--- GET FUNCTIONS ---
	float	GetPosX			( void ) const;
	float	GetPosY			( void ) const;
	float	GetPosZ			( void ) const;
	bool	GetButonRight	( void ) const;
	bool	GetButonLeft	( void ) const;
	bool	GetButonMiddle	( void ) const;

private:
	
	LPDIRECTINPUT8			m_pDI;       // The DirectInput object         
	LPDIRECTINPUTDEVICE8	m_pMouse;	

	int		m_posX;
	int		m_posY;
	int		m_posZ;
	bool	m_butonright;
	bool	m_butonleft;
	bool	m_butonmiddle;
};

#endif //INC_MOUSE_H_