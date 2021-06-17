#ifndef _NETWORK_H_
#define _NETWORK_H_

enum ENetworkResult
{
	NETRESULT_OK = 0,
	NETRESULT_ERROR
};

struct t_IPAddress
{
	char a1;
	char a2;
	char a3;
	char a4;
	unsigned short port;
};
typedef IPAddress t_IPAddress;

class Network
{
public:
	Network();
	~Network();

	ENetworkResult Init();
	ENetworkResult Term();

	ENetworkResult EnumerateAdapterIPs(IPAddress *IP, int nMaxIPs, int &nActualIPS);
};

#endif
