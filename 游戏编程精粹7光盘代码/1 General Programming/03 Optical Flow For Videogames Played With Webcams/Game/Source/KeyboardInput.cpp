#include "KeyboardInput.h"

CKeyboardInput::CKeyboardInput() 
{
	m_pDI       = NULL; // The DirectInput object         
	m_pKeyboard = NULL; // The keyboard device 
	
	for(int cont =0; cont < 256; cont++) 
		m_diks[cont]=0;
}

CKeyboardInput::~CKeyboardInput()
{
}

void CKeyboardInput::FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( m_pKeyboard ) 
        m_pKeyboard->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( m_pKeyboard );
    SAFE_RELEASE( m_pDI );
}

//-----------------------------------------------------------------------------
// Name: OnCreateDevice()
// Desc: Setups a the keyboard device using the flags from the dialog.
//-----------------------------------------------------------------------------
HRESULT CKeyboardInput::OnCreateDevice(HWND hWnd )
{
	HRESULT hr;
    DWORD   dwCoopFlags;

    FreeDirectInput();

    dwCoopFlags = DISCL_NONEXCLUSIVE;
    dwCoopFlags |= DISCL_FOREGROUND;
   
    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&m_pDI, NULL ) ) )
        return hr;
    
	// Obtain an interface to the system keyboard device.
    if( FAILED( hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL ) ) )
        return hr;
    
    // Set the data format to "keyboard format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing an array
    // of 256 bytes to IDirectInputDevice::GetDeviceState.
    if( FAILED( hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
        return hr;
    
    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    hr = m_pKeyboard->SetCooperativeLevel( hWnd, dwCoopFlags );
 
    if( FAILED(hr) )
        return hr;

    // Acquire the newly created device
    m_pKeyboard->Acquire();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: ReadImmediateData()
// Desc: Read the input device's state when in immediate mode and display it.
//-----------------------------------------------------------------------------
HRESULT CKeyboardInput::ReadImmediateData( void )
{
	HRESULT hr;
	
    if( NULL == m_pKeyboard ) 
        return S_OK;
    
    // Get the input's device state, and put the state in dims
    ZeroMemory( m_diks, sizeof(m_diks) );
    hr = m_pKeyboard->GetDeviceState( sizeof(m_diks), (LPVOID)&m_diks );
    if( FAILED(hr) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special Reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = m_pKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pKeyboard->Acquire();

        return S_OK; 
    }
			
    return S_OK;
}

char * CKeyboardInput::GetKey( void )
{
	return m_diks; 
}