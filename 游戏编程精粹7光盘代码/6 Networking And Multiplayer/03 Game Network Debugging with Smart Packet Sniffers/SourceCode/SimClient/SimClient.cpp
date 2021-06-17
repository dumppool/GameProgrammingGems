// UWServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "performance.h"
#include "NetworkManager.h"
#include "MessageMgr.h"

#define FPS_30 33.333f

NetworkManager * gpNetworkManager = NULL;
MessageMgr * gpMessageMgr = NULL;

int main(int argc, char** argv)
{
	NetworkManager netManager;
	netManager.Init();
	gpNetworkManager = &netManager;

	MessageMgr msgManager;
	msgManager.Init();
	gpMessageMgr = &msgManager;

	CPerformanceCounter frameTimeCounter;
	frameTimeCounter.Start();

	netManager.BrowseLAN();

	bool bDone = false;
	while(!bDone)
	{
		frameTimeCounter.Stop();
		float fTimeDelta = frameTimeCounter.TimeInMilliseconds();

		//Update network.
		if(gpNetworkManager)
		{
			float fRealTimeDelta = fTimeDelta / 1000.0f;
			//TRACE("Update Network: %f\n", fTimeDelta);
			gpNetworkManager->Update(fRealTimeDelta);
		}

		frameTimeCounter.Stop();
		fTimeDelta = frameTimeCounter.TimeInMilliseconds();

		frameTimeCounter.Start();

		float fTimeToSleep = FPS_30 - fTimeDelta;
		if(fTimeToSleep > 0.001f)
		{
			DWORD dwTimeToSleep = (DWORD)fTimeToSleep;
			Sleep(dwTimeToSleep);
		}		
	}

	return 0;
}

