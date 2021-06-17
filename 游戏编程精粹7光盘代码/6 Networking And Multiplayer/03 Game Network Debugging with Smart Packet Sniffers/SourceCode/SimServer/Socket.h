#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "netdefs.h"

#define MAX_BUFFER 2048

class Socket
{
public:
	Socket();
	~Socket();

	ENetworkResult Bind(IPAddress& IP);
	ENetworkResult SendTo(IPAddress& IP, void* pData, int nData, int &nActualBytes);
	ENetworkResult RecvFrom(IPAddress& IP, void* pData, int nData, int &nActualBytes);

protected:
	char m_szBuffer[MAX_BUFFER];
};

#endif
