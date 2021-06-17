// UWServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NetworkManager.h"
#include "messagemgr.h"
#include "performance.h"
#include <list>

#define FPS_30 33.333f

NetworkManager * gpNetworkMgr = NULL;
MessageMgr * gpMessageMgr = NULL;

int main(int argc, char** argv)
{
	NetworkManager netManager;
	netManager.Init();
	gpNetworkMgr = &netManager;

	MessageMgr msgManager;
	msgManager.Init();
	gpMessageMgr = &msgManager;

	CPerformanceCounter frameTimeCounter;
	frameTimeCounter.Start();

	netManager.Listen(8500);

	bool bDone = false;
	while(!bDone)
	{
		frameTimeCounter.Stop();
		float fTimeDelta = frameTimeCounter.TimeInMilliseconds();

		//Update network.
		if(gpNetworkMgr)
		{
			//TRACE("Update Network: %f\n", fTimeDelta);
			float fRealTimeDelta = fTimeDelta / 1000.0f;
			gpNetworkMgr->Update(fRealTimeDelta);
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

