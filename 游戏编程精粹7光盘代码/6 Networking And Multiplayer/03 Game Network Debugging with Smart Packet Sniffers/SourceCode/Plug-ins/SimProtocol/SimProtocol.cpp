// SimProtocol.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SimProtocol.h"

#include <winsock2.h>
#include "NetworkProtocol.h"

#define PROTOCOL_NAME "SimProtocol"

const char* g_szMessageType[] =
{
	"MSG_NONE",
	"MSG_REQUEST_JOIN",
	"MSG_REQUEST_JOIN_RESPONSE",
	"MSG_REQUEST_JOIN_RESPONSE_ACK",
	"MSG_GAME_HEARTBEAT",
	"MSG_GAME_ENTITYINFO",
	"MSG_GAME_DISCONNECT",
	"MSG_GAME_DATA",
	"MSG_RELIABLE_ACK",
	"MSG_BROWSE",
	"MSG_BROWSE_RESPONSE"
};

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C"
{


SIMPROTOCOL_API int		fnGetVersion(void)
{
	return 0;
}

SIMPROTOCOL_API const char*	fnGetProtocolName(void)
{
	return PROTOCOL_NAME;
}

SIMPROTOCOL_API int		fnGetProtocolPort(void)
{
	return 8500;
}

SIMPROTOCOL_API int		fnGetIsExclusiveAccess(void)
{
	return 0;
}

SIMPROTOCOL_API const char*	fnHandlePacket(void* pData, int nSize)
{
	t_Packet* pPacket = (t_Packet*)pData;
	
	//validate
	if(nSize > sizeof(t_Packet))
	{
		return "INVALID (0)";
	}

	if(nSize != pPacket->GetSize())
	{
		return "INVALID (1)";
	}

	EMessageType eType = pPacket->GetType();
	if(eType >= MSG_LAST)
	{
		return "INVALID (2)";
	}

	return g_szMessageType[eType];
}

};