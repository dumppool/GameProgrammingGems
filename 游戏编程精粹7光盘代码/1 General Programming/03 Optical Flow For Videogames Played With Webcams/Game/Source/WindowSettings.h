#ifndef INC_WINDOWSETTINGS_H_
#define INC_WINDOWSETTINGS_H_

class CWindowSettings
{

public:
	CWindowSettings(); 
	~CWindowSettings();  
	

	//--- SINGLETON FUNCTIONS ---
	static CWindowSettings *	GetInstance	( void );
	void						CleanUP		( void );

	void	SetWidth	( int w ) {mWidth = w; }
	void	SetHeight	( int h ) {mHeight = h; }
	
	int		GetWidth	( void ) const {return mWidth;}
	int		GetHeight	( void ) const {return mHeight;}

private:	
	static CWindowSettings*	myWindowSettings;
	int	mWidth;
	int mHeight;
	
};

#endif //INC_WINDOWSETTINGS_H_