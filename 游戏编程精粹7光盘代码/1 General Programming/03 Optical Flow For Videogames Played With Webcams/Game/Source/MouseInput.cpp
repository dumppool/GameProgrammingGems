#include "MouseInput.h"


CMouseInput::CMouseInput() 
: m_posX(0)
, m_posY(0)
, m_posZ(0)
, m_butonmiddle(false)
, m_butonleft(false)
, m_butonright(false)
, m_pMouse(NULL)
, m_pDI(NULL)
{}

CMouseInput::~CMouseInput()
{


}

void CMouseInput::FreeDirectInput()
{
   	//-------------MOUSE---------------------------//
	// Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
   
	if( m_pMouse ) 
		m_pMouse->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( m_pMouse );
    SAFE_RELEASE( m_pDI );
}

HRESULT CMouseInput::OnCreateDevice(HWND hWnd )
{
	HRESULT hr;
	DWORD   dwCoopFlags;
    FreeDirectInput();
	
	dwCoopFlags = DISCL_EXCLUSIVE;
    dwCoopFlags |= DISCL_FOREGROUND;
 
	// Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
        return hr;
    if( FAILED (hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL) ) )
		return hr;
    if( FAILED( hr = m_pMouse->SetDataFormat(&c_dfDIMouse2) ) )
		return hr;
	if (FAILED (hr = m_pMouse->SetCooperativeLevel( hWnd, dwCoopFlags )))
		return hr;
   
	// Acquire the newly created device
    if (m_pMouse) 
		m_pMouse->Acquire();
	else
		MessageBox( hWnd,"Problem with de mouse input!","Mouse", MB_ICONERROR | MB_OK );
	
    return S_OK;
}



//-----------------------------------------------------------------------------
// Name: ReadImmediateData()
// Desc: Read the input device's state when in immediate mode and display it.
//-----------------------------------------------------------------------------
HRESULT CMouseInput::ReadImmediateData( void )
{
	HRESULT       hr;
    DIMOUSESTATE2 dims2;      // DirectInput mouse state structure

    if( NULL == m_pMouse ) 
        return S_OK;
    
    // Get the input's device state, and put the state in dims
    ZeroMemory( &dims2, sizeof(dims2) );
    hr = m_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );
    if( FAILED(hr) ) 
    {
        // If input is lost then acquire and keep trying 
        hr = m_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pMouse->Acquire();
        return S_OK; 
    }
    
    // The dims structure now has the state of the mouse, so 
    // display mouse coordinates (x, y, z) and buttons.
    
	m_posX =	dims2.lX; 
	m_posY =	dims2.lY;
	m_posZ =	dims2.lZ;

	
	int num;
	num =  (dims2.rgbButtons[1] & 0x80) ? '1' : '0';
	if( num == 49)
		m_butonright = true;
	else
		m_butonright = false;

	num =  (dims2.rgbButtons[0] & 0x80) ? '1' : '0';
	if( num == 49)
		m_butonleft = true;
	else
		m_butonleft = false;
	
	num =  (dims2.rgbButtons[2] & 0x80) ? '1' : '0';
	if( num == 49)
		m_butonmiddle = true;
	else
		m_butonmiddle = false;
    
	return S_OK;
}


//-----------GET FUNCTIONS--------------------//
float CMouseInput::GetPosX( void ) const
{
	return (float)m_posX;
}
float CMouseInput::GetPosY( void ) const
{
	return (float)m_posY;
}
float CMouseInput::GetPosZ( void ) const
{
	return (float)m_posZ;
}


bool CMouseInput::GetButonRight( void ) const
{
	return m_butonright;
}
bool CMouseInput::GetButonLeft( void ) const
{
	return m_butonleft;
}
bool CMouseInput::GetButonMiddle( void ) const
{
	return m_butonmiddle;
}
//----------------------------------------------//


