#ifndef _NETWORK_CONNECTION_H_
#define _NETWORK_CONNECTION_H_

class CConnection
{
public:
	CConnection();
	~CConnection();
	void QueueReliablePacket(t_AddressedPacket* pPacket);

	struct sockaddr_in		address;
	float					lastHeartbeatReceived;
	float					lastHeartbeatSent;
	float					fArbTimePending; //amount of time in arbitration
	unsigned long			nIncomingSequenceNumber;
	unsigned long			nOutgoingSequenceNumber;

	unsigned long			nReliableIncomingSequenceNumber;
	unsigned long			nReliableOutgoingSequenceNumber;

	unsigned long			nConnectionID;
	int						heartbeatcount; //for debugging...

	PacketQueue				reliableIncomingPacketQueue;
	PacketQueue				reliableOutgoingPacketQueue;
};
typedef std::list<CConnection*> ConnectionList;

#endif
