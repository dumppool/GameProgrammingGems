#ifndef INC_WINDOW_PLAY_H_
#define INC_WINDOW_PLAY_H_

#include "Window.h"
#include "WebCamera.h"
#include "SpotManager.h"
#include "KeyboardInput.h"

class CWindow_Play: public CWindow
{

public:

	CWindow_Play(	const CAnimationImage & newImage, const std::vector<CButton>& buttons,
					unsigned int id, LPDIRECT3DDEVICE9 device);
	~CWindow_Play();

	void	Update	(	float deltaTime, int posXMouse, int posYMouse, CGameState & inGameState,  
						bool isClicking, CKeyboardInput & keyboard );
	void	Draw	( LPDIRECT3DDEVICE9 device, CDebugPrintText2D & text2d  );

	void	Start	( CGameState & inGameState );

	bool	IsAvailableWebCam	( void ) {return m_AvailableWebCam;}

private:

	WebCamera			m_webCamera;
	IDirect3DTexture9*	m_WebCamTexture;
	
	CSpotManager		m_spotManager;
	float				m_timeToUpdateWebCam;

	bool	m_pause;
	bool	m_AvailableWebCam;
	float	m_time;


private:

	void	DrawTextureFromWebCam	( LPDIRECT3DDEVICE9 device );
	HRESULT	GetTextureFromWebCam	( void );
	
	void	GetKeyboardRegions		( std::vector<float> & inKeyboardRegions, CKeyboardInput & keyboard );

};

#endif //INC_WINDOW_PLAY_H_