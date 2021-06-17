#ifndef _NETDEFS_H_
#define _NETDEFS_H_

#include <map>
#include <string>
#include <vector>


#ifdef __GNUC__
#define SOCKET int
#define INVALID_SOCKET 0
#endif


#define PORT_CLIENT 0

enum ESocketType
{
	SOCKETTYPE_RAW = 0,
	SOCKETTYPE_TCP,
	SOCKETTYPE_UDP
};

enum ENetworkResult
{
	NETRESULT_OK = 0,
	NETRESULT_ERROR,
	NETRESULT_ALREADYINITED,
	NETRESULT_ALREADYENUMERATED,
	NETRESULT_NO_DATA,
};

struct t_IPv4Address
{
	unsigned char a1;
	unsigned char a2;
	unsigned char a3;
	unsigned char a4;
	unsigned short port;
};

typedef t_IPv4Address IPAddress;

class NetworkAdapter;
typedef std::map<std::string, NetworkAdapter*> NetworkAdapters;
typedef std::vector<std::string> NetworkAdapterLookup;

class NetworkSocket;
typedef std::map<std::string, NetworkSocket*> NetworkSockets;

#endif
