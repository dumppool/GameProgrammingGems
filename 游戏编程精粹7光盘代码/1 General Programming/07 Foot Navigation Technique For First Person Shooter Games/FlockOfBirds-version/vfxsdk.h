// File Name:	VFXsdk.h		
// File Date:	May 27, 1999
// Author:		JSU
// Description:	Function prototypes for VFX sdk
// Modified by Hong Liu 06/27/00
// Modified by crt 7/19/00
//-----------------------------------------------------------------------------
// Useful Conversion constants
//-----------------------------------------------------------------------------
#define VFX_PI					3.141593f
#define VFX_RAWTODEG			180.0f / 32768.0f
#define VFX_RAWTORAD			VFX_PI / 32768.0f

// Function return defines.
#define VFX_OK					0
#define VFX_NO_TRACKER			1
#define VFX_NO_STEREO			2
#define VFX_NOTRACKER_INSTANCE	3
#define VFX_NOSTEREO_INSTANCE	3
#define VFX_TRACKER_CORRUPT		4			
#define VFX_STEREO_CORRUPT		5
#define VFX_TRACKER_NODATA		6
#define VFX_TRACKER_IDLE		7

// Function arguments.
#define VFX_DIRECTX				0
#define VFX_OVERRIDE			1
#define VFX_STEREOOUT			true
#define VFX_STEREOIN			false
#define VFX_LOCKSTEREO			true
#define VFX_UNLOCKSTEREO		false

//-----------------------------------------------------------------------------
// HMD Coordinate reporting
//-----------------------------------------------------------------------------
// The VFXGetTracking function call will return values between -32768 to 32768
// where -32768 = -179.99°, 0 = 0°, and 32768 = 180.00°; each unit change is 
// (.00549°).  The values can vary between -179.99° and +180.00° This approach allows 
// default coordinates of the head tracker to be consistent with a normal 
// joystick i.e., yaw centered is looking straight ahead. In the current 
// implementation, pitch and roll are valid over the range of +/- 70° so the 
// maximum returned value for pitch and roll is 12743(+70°) and the minimum 
// value returned is -12743(-70.00°).

#ifdef __cplusplus

 	extern "C" long VFXLoadDll( void );
	extern "C" long VFXZeroSet( void );
	extern "C" long VFXSetVideoMode( BOOL );
	extern "C" long VFXGetTracking( long *, long *, long * );
	extern "C" long VFXFreeDll( void );

	extern "C" long VFXLoadStereoDll( void );
	extern "C" long VFXSetStereoSeparation( BOOL, FLOAT );
	extern "C" long VFXSetStereoLockMode( BOOL );
	extern "C" long VFXFreeStereoDll( void );
#else

	extern long VFXLoadDll( void );
	extern long VFXZeroSet( void );
	extern long VFXSetVideoMode( BOOL );
	extern long VFXGetTracking( long *, long *, long * );
	extern long VFXFreeDll( void );

	extern long VFXLoadStereoDll( void );
	extern long VFXSetStereoSeparation( BOOL, FLOAT );
	extern long VFXSetStereoLockMode( BOOL );
	extern long VFXFreeStereoDll( void );

#endif