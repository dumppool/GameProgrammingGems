#include "StdAfx.h"
#include "NetworkConnection.h"
#include "NetworkTweakables.h"

CConnection::CConnection():
lastHeartbeatReceived(0.0f),
lastHeartbeatSent(0.0f),
fArbTimePending(0.0f),
nIncomingSequenceNumber(0),
nOutgoingSequenceNumber(0),
nReliableIncomingSequenceNumber(0),
nReliableOutgoingSequenceNumber(0),
nConnectionID(0),
heartbeatcount(0)
{
	memset(&address, 0, sizeof(struct sockaddr_in));
}

CConnection::~CConnection()
{
	//TODO: clear out any reliable packets
	PacketQueue::iterator iter = reliableOutgoingPacketQueue.begin();
	while(iter != reliableOutgoingPacketQueue.end())
	{
		delete (*iter);
		++iter;
	}
}

void CConnection::QueueReliablePacket(t_AddressedPacket* pPacket)
{
	//Set up the packet to be sent the next time we update our reliable packets.
	pPacket->fResendTimeout = NetworkTweakables::GetInstance().GetReliablePacketResendTime();

	pPacket->payload.header.sequenceNum = nReliableOutgoingSequenceNumber;
	reliableOutgoingPacketQueue.push_back(pPacket);
	++nReliableOutgoingSequenceNumber;
}