
#pragma once

#include "resource.h"

class XenoTestbedWindow;

class XenoTestbedApp
{

public:

	XenoTestbedApp();

public:

	BOOL InitInstance(HINSTANCE hInstance);
	int ExitInstance();

	// Implementation
	BOOL PumpWaitingMessages();
	BOOL RunMainLoop();

	XenoTestbedWindow* m_mainWindow;
};

extern XenoTestbedApp theApp;