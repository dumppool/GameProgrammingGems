// File Name:	VFXsdk.c		
// File Date:	May 27, 1999
// Author:		JSU
// Description:	Functions to load the VFXDriver.dll, receive tracking
//				info, and Free the VXFDriver.dll
//				Functions to load the VFXDraw,.dll, set up the IPD seperation 
//				value, and then free the VFXDraw.dll
// Modified by Hong Liu 06/27/00
// Modified by crt 6/19/00
#include <windows.h>
#include "vfxsdk.h"

// Typedefs for VFXTracker driver
typedef long	(__cdecl *DEFVFXOPEN) ( void );
typedef long	(__cdecl *DEFVFXCLOSE) ( void );
typedef long	(__cdecl *DEFVFXZERO) ( void );
typedef long	(__cdecl *DEFVFXSETVIDEO) ( BOOL );
typedef long	(__cdecl *DEFVFXGETTRACK) ( long *,long *,long * );

// Typedefs for VFXStereo driver 
#define VFXCFG_STEREOOUT 0x400
typedef long	(__cdecl *VFXSTEREOLOCKMODE) ( BOOL );	
typedef long	(__cdecl *DEFVFXSETCONFIGBITS) ( long );
typedef long	(__cdecl *DEFVFXGETCONFIGBITS) ( void );
typedef long	(__cdecl *DEFVFXSETIPDPERCENT) ( FLOAT );

// Global definitions
HANDLE				VFXTrackerInstance	 = 0;
DEFVFXOPEN			RealVFXOpen			 = 0;
DEFVFXCLOSE			RealVFXClose		 = 0;
DEFVFXZERO			RealVFXZeroSet		 = 0;
DEFVFXSETVIDEO		RealVFXSetVideoMode	 = 0;
DEFVFXGETTRACK		RealVFXGetTracking	 = 0;

HMODULE				VFXStereoInstance	 = 0;					
VFXSTEREOLOCKMODE	RealVFXStereoLockMode= 0;
DEFVFXSETCONFIGBITS RealVFXSetConfigBits = 0;
DEFVFXGETCONFIGBITS	RealVFXGetConfigBits = 0;
DEFVFXSETIPDPERCENT RealVFXSetIPDPercent = 0;

// Name:	VFXLoadDll
// Desc:	loading the vfx tracker driver and initializes the tracker system
// Returns:	VFX_OK
//			VFX_NO_TRACKER 
//			VFX_TRACKER_CORRUPT	
//
long	VFXLoadDll(void){

	VFXTrackerInstance = LoadLibrary( "VFXDrive.dll" );
	if( VFXTrackerInstance == NULL) 
		return VFX_NO_TRACKER;

	RealVFXOpen = (DEFVFXOPEN) GetProcAddress(VFXTrackerInstance, "VFXOpen" );
	RealVFXClose = (DEFVFXCLOSE) GetProcAddress(VFXTrackerInstance, "VFXClose" );
	RealVFXZeroSet = (DEFVFXZERO) GetProcAddress(VFXTrackerInstance, "VFXZeroSet" );
	RealVFXSetVideoMode = (DEFVFXSETVIDEO) GetProcAddress(VFXTrackerInstance, "VFXSetVideoMode" );
	RealVFXGetTracking = (DEFVFXGETTRACK) GetProcAddress(VFXTrackerInstance, "VFXGetTracking" );

	if( !RealVFXGetTracking || !RealVFXOpen || !RealVFXClose || !RealVFXZeroSet || !RealVFXSetVideoMode)
		return VFX_TRACKER_CORRUPT;

	RealVFXOpen();

	return  VFX_OK;
}
		
// Name:	VFXGetTracking
// Desc:	Reads in the current yaw, pitch, and roll values of the tracking system
//			in the format of raw data(-32768~32768)
// Retu:	VFX_OK
//			VFX_TRACKER_IDLE
//			VFX_TRACKER_NODATA
//			VFX_TRACKER_CORRUPT
//
long	VFXGetTracking( long *yaw, long *pitch, long *roll ){
long	Ret;
	if( RealVFXGetTracking ) {
		Ret = RealVFXGetTracking( yaw, pitch, roll );
		if( Ret == 1 ) 
			return VFX_OK;
		else
		if( Ret == 2 || Ret == 3 ) 
			return VFX_TRACKER_IDLE;
		return VFX_TRACKER_NODATA;
		}
	return VFX_TRACKER_CORRUPT;
}

// Name:    VFXZeroSet
// Desc:	Establishes a new zero orientation location based on what the tracker is currently
//			reporting as tracking data.
// Retu:	VFX_OK
//			VFX_TRACKER_CORRUPT   
//
long	VFXZeroSet( void ){
	if( !RealVFXZeroSet ) 
		return VFX_TRACKER_CORRUPT;

	RealVFXZeroSet( );
	return VFX_OK;
}

// Name:    VFXSetVideoMode
// Desc:	Will put the Trackers video system into directx Stereo mode such that the driver 
//			will decide when to perform stereo.  Or the Override mode which allows an older 
//			stereo applicaion can perform stereo allowing the Video system of the tracker to 
//			be switched into and out of stereo mode.
// Retu:	VFX_OK
//			VFX_TRACKER_CORRUPT   
//
long	VFXSetVideoMode( BOOL mode ){
	if( !RealVFXSetVideoMode ) 
		return VFX_TRACKER_CORRUPT;

	RealVFXSetVideoMode( mode );
	return VFX_OK;
}

// Name:	VFXFreeDll
// Desc:	Frees the dynamically loaded VFX tracker driver.
// Retu:    VFX_OK
//			VFX_NO_INSTANCE
//
long	VFXFreeDll( void ){
	if( VFXTrackerInstance ) {
		RealVFXClose();	
		FreeLibrary( VFXTrackerInstance );
		VFXTrackerInstance = NULL;
		return VFX_OK;		
	}
	return VFX_NOTRACKER_INSTANCE;
}

// Name:    VFXLoadStereoDll
// Desc:    Loads the VFX stereo driver, gets all addresses.
// Retu:	VFX_OK 
//			VFX_NO_STEREO
//			VFX_STEREO_CORRUPT
//	
long	VFXLoadStereoDll(void){
	VFXStereoInstance = LoadLibrary( "VFXDraw.dll" );	
	if(VFXStereoInstance==NULL)
		 return VFX_NO_STEREO;

	RealVFXStereoLockMode= (VFXSTEREOLOCKMODE) GetProcAddress( VFXStereoInstance,"VFXLockOutMode" );
	RealVFXSetConfigBits = (DEFVFXSETCONFIGBITS) GetProcAddress( VFXStereoInstance,"VFXSetConfigBits" );
	RealVFXGetConfigBits = (DEFVFXGETCONFIGBITS) GetProcAddress( VFXStereoInstance,"VFXGetConfigBits" );
	RealVFXSetIPDPercent = (DEFVFXSETIPDPERCENT) GetProcAddress( VFXStereoInstance,"VFXSetIPDPercent" );

	if( !RealVFXStereoLockMode || !RealVFXSetConfigBits || !RealVFXGetConfigBits || !RealVFXSetIPDPercent )
		return VFX_STEREO_CORRUPT;

	return  VFX_OK;
}

// Name:	VFXSetStereoSeparation
// Desc:	sets up the stereo driver's mode of stereoscopy and the IPD seperation amount
//			mode - false perform stereoscopy into the apps. environment.
//				   true perform stereoscopy out of the apps. environment.
//			ipd - a float value representing the amount of left and right seperation
//				   of the apps. viewing of the environment. 
// Retu:	VFX_OK
//			VFX_STEREO_CORRUPT
//
long	VFXSetStereoSeparation( BOOL mode, FLOAT ipd ){
long	CfgBits;

	if( !RealVFXGetConfigBits && !RealVFXSetConfigBits && !RealVFXSetIPDPercent )
		return VFX_STEREO_CORRUPT;
 
	CfgBits=RealVFXGetConfigBits( );
	if( mode ) 
		CfgBits |= VFXCFG_STEREOOUT;
	else
		CfgBits &= ~VFXCFG_STEREOOUT;

	RealVFXSetConfigBits(CfgBits);
  	RealVFXSetIPDPercent(ipd);

	return VFX_OK;
}

// Name:    VFXSetStereoLockMode
// Desc:	Locks out the stereo driver from performing stereoscopy.
//			mode = false stereo generation can be performed by the stereo driver.
//				the driver will determine to perform this based on the trackers stereo switches and 
//				the apps. support of DirectX.
//			mode = true stereo generation by the driver is now lockout or disabled.
//				No stereo images will be created during this state.
// Retu:	VFX_OK
//			VFX_STEREO_CORRUPT   
//
long	VFXSetStereoLockMode( BOOL mode ){
	if( !RealVFXStereoLockMode ) 
		return VFX_STEREO_CORRUPT;

	RealVFXStereoLockMode( mode );
	return VFX_OK;
}

// Name:    VFXFreeStereoDll
// Desc:    Frees the dynamically loaded vfxstereo driver.
// Retu:	VFX_OK
//			VFX_NO_INSTANCE
//
long	VFXFreeStereoDll(void){
	if( VFXStereoInstance ){
		FreeLibrary(VFXStereoInstance);
		VFXStereoInstance = NULL;
		return VFX_OK;
	}
	return VFX_NOSTEREO_INSTANCE;
}
