#ifndef _NETWORKSOCKET_H_
#define _NETWORKSOCKET_H_

#include "netdefs.h"

class NetworkSocket
{
public:
	NetworkSocket(){}
	virtual ~NetworkSocket(){}
	virtual ENetworkResult SendTo(IPAddress* pAddress, void* pData, int nDataLen) = 0;
	virtual ENetworkResult RecvFrom(IPAddress &address, void* pData, int nDataLen, int &nBytesReceived) = 0;

protected:
};

#endif
