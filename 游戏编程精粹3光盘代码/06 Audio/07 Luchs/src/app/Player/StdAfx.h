// stdafx.h : Include-Datei f�r Standard-System-Include-Dateien,
//  oder projektspezifische Include-Dateien, die h�ufig benutzt, aber
//      in unregelm��igen Abst�nden ge�ndert werden.
//

#if !defined(AFX_STDAFX_H__ABC1C45D_32CE_4937_84F7_4048ED30FABD__INCLUDED_)
#define AFX_STDAFX_H__ABC1C45D_32CE_4937_84F7_4048ED30FABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header nicht einbinden

#include <afxwin.h>         // MFC-Kern- und -Standardkomponenten
#include <afxext.h>         // MFC-Erweiterungen
#include <afxdtctl.h>		// MFC-Unterst�tzung f�r allgemeine Steuerelemente von Internet Explorer 4
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC-Unterst�tzung f�r g�ngige Windows-Steuerelemente
#endif // _AFX_NO_AFXCMN_SUPPORT


// ignore template warnings
#pragma warning (disable:4786)
#pragma warning (disable:4251)
#pragma warning (disable:4275)

#include "atomos.h"

#include "sphinxmmos_includes.h"



#define CMD_UPDATE_PATCHES	10


#include "SphinxMMOSPlayer.h"
#include "SphinxMMOSPlayerDoc.h"
#include "SphinxMMOSPlayerMainFrame.h"
#include "SphinxMMOSPlayerView.h"
//#include "WaveView.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f�gt unmittelbar vor der vorhergehenden Zeile zus�tzliche Deklarationen ein.

#endif // !defined(AFX_STDAFX_H__ABC1C45D_32CE_4937_84F7_4048ED30FABD__INCLUDED_)
