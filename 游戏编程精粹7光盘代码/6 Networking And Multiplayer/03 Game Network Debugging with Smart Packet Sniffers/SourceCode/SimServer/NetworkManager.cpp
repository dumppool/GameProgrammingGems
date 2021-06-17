#include "stdafx.h"
#include "NetworkManager.h"
#include "Performance.h"
#include "NetworkTweakables.h"

#include "MessageMgr.h"
#include "SysMessage.h"
extern MessageMgr *gpMessageMgr;

//For USES_CONVERSION macro
#include <atlbase.h>

NetworkManager::NetworkManager():
m_eState(NET_STATE_IDLE),
m_hSocket(INVALID_SOCKET),
m_nCurrentConnectionID(0)
{}

NetworkManager::~NetworkManager()
{
	// Just in case we need to clean up.
	Term();
}

bool NetworkManager::Init()
{
	WSADATA w;
	int error = WSAStartup (0x0202, &w);   // Fill in w

	if (error)
	{ // there was an error
		return false;
	}

	if (w.wVersion != 0x0202)
	{ // wrong WinSock version!
		return false;
	}

	//TODO: pre-allocate some t_addressedpackets...

	return true;
}

bool NetworkManager::Term()
{
	if(m_hSocket != INVALID_SOCKET)
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}

	//delete all connections
	ConnectionList::iterator iter = m_ConnectionList.begin();
	while(iter != m_ConnectionList.end())
	{
		//TODO: delete the reliable packets...

		delete (*iter);
		++iter;
	}

	//Clear our freelist
	PacketQueue::iterator iterPacketFree = m_PacketFreeList.begin();
	while(iterPacketFree != m_PacketFreeList.end())
	{
		delete (*iterPacketFree);
		++iterPacketFree;
	}

	//FIXME: check for an error code here...
	WSACleanup();

	return true;
}

void NetworkManager::Update(float fTimeDelta)
{
	if(m_eState != NET_STATE_LISTENING)
	{
		return;
	}
	
	CPerformanceCounter counter;
	counter.Start();

	int nRecThisFrame = 0;

	while(true)
	{
		bool bReceived = HandleIncomingPacket();
		if(!bReceived)
		{
			break;
		}
		++nRecThisFrame;

		counter.Stop();
		//if(counter.TimeInMilliseconds() > 16.67f) // ~(1/60)
		TRACE("time slice: %f\n", counter.TimeInMilliseconds());
		if(counter.TimeInMilliseconds() > 66.66f) // ~(1/15)
		{
			//TODO: We should probably send off a warning if this keeps happening?
			TRACE("NetworkManager::Update we're over our time slice for this update!\n");
			break;
		}
	}

	//TRACE("pack %d\n", nRecThisFrame);

	// check on our arbitrations
	UpdatePendingConnections(fTimeDelta);

	//check our connection list for time outs
	//check our connection list to see if we need to send any heartbeats
	UpdateConnections(fTimeDelta);

	UpdateReliableOutgoingPackets(fTimeDelta);

	//If we need to send any packets, let's do so...
	FlushOutgoingPackets();
}

bool NetworkManager::Connect(const char* szAddress, unsigned short port)
{
	if((szAddress == NULL) || (szAddress[0] == '\0') || (port == 0))
	{
		return false;
	}

	return true;
}


bool NetworkManager::Listen(unsigned short port)
{
	//if we're not idle, then bail
	if(m_eState != NET_STATE_IDLE)
	{
		return false;
	}

	//create the socket
	m_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_hSocket == INVALID_SOCKET)
	{
		TRACE("socket() failed. (%d)\n", WSAGetLastError());	
		return false;
	}

	//bind the socket
	/* Set family and port */
	struct sockaddr_in server;
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	if (bind(m_hSocket, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1)
	{
		TRACE("Could not bind name to socket. (%d)\n", WSAGetLastError());					
		return false;
	}

	//set sockopt
	int x = 1;
	if (setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&x, sizeof(x))== SOCKET_ERROR) 
	{
		TRACE("setsockopt failed. (%d)\n", WSAGetLastError());	
		return false;
	}

	if (setsockopt(m_hSocket, SOL_SOCKET, SO_BROADCAST, (char *)&x, sizeof(x))== SOCKET_ERROR) 
	{
		TRACE("setsockopt failed. (%d)\n", WSAGetLastError());	
		return false;
	}

	//set nonblocking
	u_long iMode = 1;
	if(ioctlsocket(m_hSocket, FIONBIO, &iMode))
	{
		TRACE("ioctlsocket failed. (%d)\n", WSAGetLastError());	
		return false;
	}

	m_eState = NET_STATE_LISTENING;

	return false;
}

bool NetworkManager::ValidateAddress(struct sockaddr_in *pAddress)
{
	if(pAddress == NULL)
	{
		return false;
	}

	ConnectionList::iterator iter = m_ConnectionList.begin();
	while(iter != m_ConnectionList.end())
	{
		if(memcmp(pAddress, &((*iter)->address), sizeof(struct sockaddr_in)) == 0)
		{
			return true;
		}

		++iter;
	}

	return false;
}

bool NetworkManager::ValidatePacket(t_AddressedPacket* packet, int nSize)
{
	//perhaps do checksum validation here?
	if( !ValidateChecksum((t_Packet*)&(packet->payload)) )
	{
		return false;
	}

	//
	if(packet->GetType() == MSG_NONE)
	{
		return false;
	}

	//check for size validation
	if(nSize != packet->GetPayloadSize())
	{
		//This will happen if the packet is corrupt, or more likely, 
		//protocol is out of date between client/server
		TRACE("packet rejected due to invalid size... Type: %d (%d != %d)\n", packet->GetType(), nSize, packet->GetPayloadSize());
		return false;
	}

	return true;
}

unsigned long NetworkManager::CreateChecksum(const unsigned char* pBuffer, unsigned int nSize)
{

	unsigned long crc = 0xFFFFFFFF;
	const unsigned char* ptr = pBuffer;
	while (nSize--) 
	{
		crc ^= ((unsigned long)(*ptr++) << 24);
		for (int i = 0; i < 8; i++) //8 bits, since this is a char.
		{
			if (crc & 0x80000000) 
			{
				crc = (crc << 1) ^ 0x2D8FA9E4; //(randomly chosen)
			}
			else 
			{
				crc <<= 1;
			}
		}
	}

	return crc;
}

bool NetworkManager::ChecksumPacket(t_Packet* packet)
{
	packet->header.crc = 0;
	packet->header.crc = CreateChecksum( (unsigned char*)packet, packet->GetSize());

	return true;
}

bool NetworkManager::ValidateChecksum(t_Packet* packet)
{
	unsigned long crc = packet->header.crc;

	// We require that a checksum has been set.
	if(crc == 0)
	{
		return false;
	}

	packet->header.crc = 0;
	if(CreateChecksum( (unsigned char*)packet, packet->GetSize()) == crc)
	{
		packet->header.crc = crc;
		return true;
	}

	return false;
}

bool NetworkManager::IsPendingConnection(struct sockaddr_in *pAddress)
{
	ConnectionList::iterator iter = m_ConnectionPendingList.begin();
	while(iter != m_ConnectionPendingList.end())
	{
		if(memcmp(pAddress, &((*iter)->address), sizeof(struct sockaddr_in)) == 0)
		{
			return true;
		}

		++iter;
	}

	return false;
}

bool NetworkManager::IsFullyConnected(struct sockaddr_in *pAddress)
{
	ConnectionList::iterator iter = m_ConnectionList.begin();
	while(iter != m_ConnectionList.end())
	{
		if(memcmp(pAddress, &((*iter)->address), sizeof(struct sockaddr_in)) == 0)
		{
			return true;
		}

		++iter;
	}

	return false;
}

CConnection* NetworkManager::FindPendingConnection(struct sockaddr_in *pAddress)
{
	ConnectionList::iterator iter = m_ConnectionPendingList.begin();
	while(iter != m_ConnectionPendingList.end())
	{
		if(memcmp(pAddress, &((*iter)->address), sizeof(struct sockaddr_in)) == 0)
		{
			return (*iter);
		}

		++iter;
	}

	return NULL;
}

bool NetworkManager::RemovePendingConnection(CConnection* pConnection)
{
	assert(pConnection);
	if(pConnection == NULL)
	{
		return false;
	}

	m_ConnectionPendingList.remove(pConnection);
	return true;
}

CConnection* NetworkManager::FindConnection(struct sockaddr_in *pAddress)
{
	ConnectionList::iterator iter = m_ConnectionList.begin();
	while(iter != m_ConnectionList.end())
	{
		if(memcmp(pAddress, &((*iter)->address), sizeof(struct sockaddr_in)) == 0)
		{
			return (*iter);
		}

		++iter;
	}

	return NULL;
}

bool NetworkManager::RemoveConnection(CConnection* pConnection)
{
	assert(pConnection);
	if(pConnection == NULL)
	{
		return false;
	}

	m_ConnectionList.remove(pConnection);
	return true;
}

bool NetworkManager::UpdatePendingConnections(float fTimeDelta)
{
	ConnectionList::iterator iter = m_ConnectionPendingList.begin();
	while(iter != m_ConnectionPendingList.end())
	{
		(*iter)->fArbTimePending += fTimeDelta;
		if((*iter)->fArbTimePending >= 5.0f)
		{
			//has arbitration taken longer than 5 seconds?
			//if so, drop this pending connection...
			TRACE("Dropping join arbitration for %s\n", inet_ntoa((*iter)->address.sin_addr) );

			//Release memory for this connection
			delete (*iter);

			//Clear this out of out pending list...
			m_ConnectionPendingList.erase(iter);
			iter = m_ConnectionPendingList.begin();
			continue;
		}

		++iter;
	}

	return false;
}

bool NetworkManager::UpdateConnections(float fTimeDelta)
{
	ConnectionList::iterator iter = m_ConnectionList.begin();
	while(iter != m_ConnectionList.end())
	{
		//check this conn for time out
		(*iter)->lastHeartbeatReceived += fTimeDelta;
		if((*iter)->lastHeartbeatReceived >= 5.0f)
		{
			//Inform the game...
			t_SysMessage msgSystem;
			msgSystem.nMsgID = SYS_MSG_PLAYER_CONNECTION;
			msgSystem.sysMessagePlayerConnection.eCode = PLAYER_DISCONNECTED;
			msgSystem.sysMessagePlayerConnection.nID = (*iter)->nConnectionID;
			gpMessageMgr->SendSystemMessage( &msgSystem );

			TRACE("Dropping a client due to timeout...\n");
			m_ConnectionList.erase(iter);
			iter = m_ConnectionList.begin();
			continue;
		}
		else
		{
			//check this conn to see if we owe it a heartbeat
			(*iter)->lastHeartbeatSent += fTimeDelta;
			if( (*iter)->lastHeartbeatSent >= 1.0f)
			{
				t_AddressedPacket *pPacket = CreatePacket();
				assert(pPacket);

				if(pPacket)
				{
					(*iter)->lastHeartbeatSent = 0;
					memcpy(&pPacket->address, &((*iter)->address) , sizeof(struct sockaddr_in));
					pPacket->SetType(MSG_GAME_HEARTBEAT);
					pPacket->payload.msgHeartbeat.nReserved = 0;
					(*iter)->heartbeatcount++; //debug info.
					if(QueuePacket(pPacket) == false)
					{
						//Error...
					}
					else
					{
						TRACE("Queue heartbeat for client...(%d)\n", (*iter)->heartbeatcount);
					}
				}
			}
		}

		++iter;
	}

	return true;
}

bool NetworkManager::HandleIncomingPacket()
{
	int addr_length = (int)sizeof(struct sockaddr_in);

	//Grab a packet
	t_AddressedPacket *pPacket = CreatePacket();
	assert(pPacket);
	if(pPacket == NULL)
	{
		return false;
	}

	int nBytesReceived = recvfrom(	m_hSocket, 
									(char*)&pPacket->payload, 
									pPacket->payload.GetSize(), 
									0, 
									(struct sockaddr *)&pPacket->address, 
									&addr_length);

	if(nBytesReceived == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if((err != WSAEWOULDBLOCK) &&   //This can happen if there's nothing on the wire.
			(err != WSAECONNRESET))     //This can happen if a packet was sent to the other side, but failed
										//delivery... aka the client shutdown. Ignore this error for UDP.
		{
			TRACE("Error on recvfrom! (code: %d)\n", WSAGetLastError());
		}

		m_PacketFreeList.push_back(pPacket);
		return false;			
	}

	if(nBytesReceived == 0)
	{
		// There's no data on the wire.
		m_PacketFreeList.push_back(pPacket);
		return false;
	}

	// Let's do our sanity checks in here.
	if(!ValidatePacket(pPacket, nBytesReceived))
	{
		//TRACE("packet failed to validate...\n");
		m_PacketFreeList.push_back(pPacket);
		return false;
	}

	//TRACE("Incoming packet from %s of type (%d)\n", inet_ntoa(pPacket->address.sin_addr), pPacket->GetType());

	if(pPacket->IsOOB())
	{
		//is this an unconnected packet? (OOB)
		HandleOOBPacket(pPacket);
		m_PacketFreeList.push_back(pPacket);
	}
	else //If this is not an OOB packet then it can only come from a fully connected address:port.
	if( IsFullyConnected( &(pPacket->address) ))
	{
		//Pass it to a handler
		HandlePacket(pPacket);
		m_PacketFreeList.push_back(pPacket);

	}
	else
	{
		m_PacketFreeList.push_back(pPacket);
		return false;
	}

	return true;
}

bool NetworkManager::HandleOOBPacket(t_AddressedPacket* pPacket)
{
	//yes, then validate it against our pendings..
	if(IsPendingConnection( &(pPacket->address) ))
	{
		//Ok, this address is in our list, the only valid packet type is a join response ack.
		if(pPacket->GetType() == MSG_REQUEST_JOIN_RESPONSE_ACK)
		{
			//ok, add this to our connection list
			CConnection* pConnection = FindPendingConnection( &(pPacket->address) );
			if(pConnection)
			{
					TRACE("Received MSG_REQUEST_JOIN_RESPONSE_ACK from %s\n", inet_ntoa(pPacket->address.sin_addr));

				//remove it from our pending list
				RemovePendingConnection(pConnection);

				//add to our connections list
				//TODO: handle the case where the server has been up long enough
				// to serve up 4 billion connection IDs.
				pConnection->nConnectionID = ++m_nCurrentConnectionID; //set our connection ID
				m_ConnectionList.push_back(pConnection);

				//send a heartbeat (the client may use this is a validation.
				t_AddressedPacket* pNewPacket = CreatePacket();
				assert(pNewPacket);
				if(pNewPacket == NULL)
				{
					return false;
				}

				memcpy( &(pNewPacket->address), &(pPacket->address), sizeof(sockaddr_in));
				pNewPacket->SetType(MSG_GAME_HEARTBEAT);
				QueuePacket(pNewPacket);

				t_SysMessage msgSystem;
				msgSystem.nMsgID = SYS_MSG_PLAYER_CONNECTION;
				msgSystem.sysMessagePlayerConnection.eCode = PLAYER_CONNECTED;
				msgSystem.sysMessagePlayerConnection.nID = pConnection->nConnectionID;
				gpMessageMgr->SendSystemMessage( &msgSystem );

				return true;
			}
			else
			{
				TRACE("A pending connection sent us this: %d but we failed to find a connection\n", pPacket->GetType());
			}
			//or else we're rejecting this one...
		}

		//else reject it
		return false;
	}
	else if( IsFullyConnected( &(pPacket->address) ))
	{
		//Currently we don't support any OOB packets for fully connected hosts
		//Perhaps later there may be some, such as disconnect messages.
		//Right now this basically just wastes CPU cycles that it doesn't have to.
	}
	else
	{
		//If it's not in any of our lists, then the only acceptable type is a Join request.
		//We could only get to this code if the address is not listed in our pending, or
		//fully connected lists..

		switch(pPacket->GetType())
		{
		case MSG_REQUEST_JOIN:
			{
				TRACE("Received MSG_REQUEST_JOIN from %s\n", inet_ntoa(pPacket->address.sin_addr));
				//create new connection...
				CConnection* pConnection = new CConnection;
				assert(pConnection);

				//Save the address
				memcpy( &(pConnection->address), &(pPacket->address), sizeof(sockaddr_in));

				//add it to our list
				m_ConnectionPendingList.push_back(pConnection);

				//send a response
				t_AddressedPacket* pNewPacket = CreatePacket();
				memcpy( &(pNewPacket->address), &(pPacket->address), sizeof(sockaddr_in));
				pNewPacket->SetType(MSG_REQUEST_JOIN_RESPONSE);
				pNewPacket->payload.msgJoinResponse.nVersion = PROTOCOL_VERSION;
				pNewPacket->payload.msgJoinResponse.eCode = 
					(pPacket->payload.msgJoinRequest.nVersion == PROTOCOL_VERSION) ? JOIN_CODE_OK : JOIN_CODE_DENIED;

				QueuePacket(pNewPacket);

				return true;
			}
			break;
		case MSG_BROWSE:
			return HandleBrowse(pPacket);

			default:
				break;
		}
	}

	//if it gets here, reject it..
	return false;
}

bool NetworkManager::HandlePacket(t_AddressedPacket* pPacket)
{
	switch(pPacket->GetType())
	{
	case MSG_GAME_HEARTBEAT:
		return HandleHeartbeat(pPacket);
	case MSG_GAME_DATA:
		return HandleGameData(pPacket);
	case MSG_RELIABLE_ACK:
		return HandleReliableAck(pPacket);
	default:
		return false;
	}
}

bool NetworkManager::HandleHeartbeat(t_AddressedPacket* pPacket)
{
	CConnection* pConnection = FindConnection( &(pPacket->address) );
	if(pConnection)
	{
		TRACE("Received HEARTBEAT from %s\n", inet_ntoa(pPacket->address.sin_addr));
		pConnection->lastHeartbeatReceived = 0;
		return true;
	}

	return false;
}

bool NetworkManager::HandleGameData(t_AddressedPacket* pPacket)
{
	CConnection* pConnection = FindConnection( &(pPacket->address) );
	if(pConnection)
	{
		//Pass this up to the Message Mgr...
		pPacket->payload.msgGame.nID = pConnection->nConnectionID;
		gpMessageMgr->DeliverMessage( &(pPacket->payload.msgGame) );
		return true;
	}

	return false;
}

bool NetworkManager::HandleReliableAck(t_AddressedPacket* pPacket)
{
	CConnection* pConnection = FindConnection( &(pPacket->address) );
	if(pConnection)
	{
		TRACE("We got an Ack for packet: %d\n", pPacket->payload.msgAck.nSequenceNumber);

		//OK we got an Ack for this one. Clear it out of our list.
		PacketQueue::iterator iter = pConnection->reliableOutgoingPacketQueue.begin();
		while(iter != pConnection->reliableOutgoingPacketQueue.end())
		{
			if(pPacket->payload.msgAck.nSequenceNumber == (*iter)->payload.header.sequenceNum )
			{
				pConnection->reliableOutgoingPacketQueue.erase( iter );
				return true;
			}

			++iter;
		}	
	}

	return false;
}

bool NetworkManager::QueueGameMessageForConnection(unsigned long nID, t_GameMessage *pMessage, unsigned long nFlags)
{
	//An std::map might be faster for a large number of connections, but this should be fine.

	ConnectionList::iterator iter = m_ConnectionList.begin();
	while(iter != m_ConnectionList.end())
	{
		if( (*iter)->nConnectionID == nID )
		{
			t_AddressedPacket *pPacket = CreatePacket();
			assert(pPacket);
			if(pPacket)
			{
				//Address it
				memcpy( &(pPacket->address), &((*iter)->address), sizeof(struct sockaddr_in));

				//Set the type and copy the data.
				pPacket->SetType(MSG_GAME_DATA);
				memcpy( &(pPacket->payload.msgGame), pMessage, sizeof(t_GameMessage));

				pPacket->payload.header.flags = nFlags;

				if(nFlags & MSG_FLAG_RELIABLE)
				{
					(*iter)->QueueReliablePacket(pPacket);
				}
				else
				{
					//Queue it.
					QueuePacket(pPacket);
				}

				return true;
			}
		}

		++iter;
	}

	return false;
}

bool NetworkManager::HandleBrowse(t_AddressedPacket* pPacket)
{
	//respond
	t_AddressedPacket *pNewPacket = CreatePacket();
	assert(pNewPacket);
	if(pNewPacket)
	{
		//Address it
		memcpy( &(pNewPacket->address), &(pPacket->address), sizeof(struct sockaddr_in));

		//Set the type and copy the data.
		pNewPacket->SetType(MSG_BROWSE_RESPONSE);

		//TODO: set up the packet data here
		pNewPacket->payload.msgBrowseResponse.nVersion = PROTOCOL_VERSION;
		pNewPacket->payload.msgBrowseResponse.nNumPlayers = (unsigned short)m_ConnectionList.size();
		//char	szGameDesc[16];
		DWORD dwSize = 16;
		wchar_t szwName[16];
		GetComputerName(szwName, &dwSize);

		USES_CONVERSION;

		char* pName = W2A(szwName);
		sprintf(pNewPacket->payload.msgBrowseResponse.szGameDesc, pName, 15);
		
		//Queue it.
		QueuePacket(pNewPacket);

		return true;
	}

	return false;
}

bool NetworkManager::QueuePacket(t_AddressedPacket* pPacket)
{
	if(pPacket == NULL)
	{
		return false;
	}

	m_OutgoingPackets.push_back(pPacket);

	return true;
}

bool NetworkManager::UpdateReliableOutgoingPackets(float fDeltaTime)
{
	ConnectionList::iterator iterConnection = m_ConnectionList.begin();
	while(iterConnection != m_ConnectionList.end())
	{
		PacketQueue::iterator iterPacket = (*iterConnection)->reliableOutgoingPacketQueue.begin();
		while(iterPacket != (*iterConnection)->reliableOutgoingPacketQueue.end())
		{
			(*iterPacket)->fResendTimeout += fDeltaTime;
			if((*iterPacket)->fResendTimeout >= NetworkTweakables::GetInstance().GetReliablePacketResendTime())
			{
				//We could be timing this as well, but the timer above should keep us out of too much trouble.
				TRACE("Sending: %d to %s [Reliable]\n",(*iterPacket)->GetType(), inet_ntoa((*iterPacket)->address.sin_addr));

				// Create a checksum for this packet.
				ChecksumPacket( (t_Packet*)&((*iterPacket)->payload) );

				//Send it...
				int nNumBytes = sendto(m_hSocket, 
					(char*)&((*iterPacket)->payload), 
					(*iterPacket)->payload.GetSize(), 
					0, 
					(struct sockaddr*)&((*iterPacket)->address), 
					sizeof(struct sockaddr_in) 
					);

				if( (nNumBytes == SOCKET_ERROR) || (nNumBytes == 0) )
				{
					TRACE("client: %s:%d\n", inet_ntoa((*iterPacket)->address.sin_addr), ntohs((*iterPacket)->address.sin_port) );
					TRACE("sendto error! (%d)\n", WSAGetLastError());
					assert(0);
				}

				(*iterPacket)->fResendTimeout = 0;
			}

			++iterPacket;
		}

		++iterConnection;
	}

	return false;
}

bool NetworkManager::FlushOutgoingPackets()
{
	PacketQueue::iterator iter = m_OutgoingPackets.begin();
	while(iter != m_OutgoingPackets.end())
	{
		//We could be timing this as well, but the timer above should keep us out of too much trouble.
		TRACE("Sending: %d to %s\n",(*iter)->GetType(), inet_ntoa((*iter)->address.sin_addr));

		// Create a checksum for this packet.
		ChecksumPacket( (t_Packet*)&((*iter)->payload) );

		//Send it...
		int nNumBytes = sendto(m_hSocket, 
					(char*)&((*iter)->payload), 
					(*iter)->payload.GetSize(), 
					0, 
					(struct sockaddr*)&((*iter)->address), 
					sizeof(struct sockaddr_in) 
					);

		if( (nNumBytes == SOCKET_ERROR) || (nNumBytes == 0) )
		{
			TRACE("client: %s:%d\n", inet_ntoa((*iter)->address.sin_addr), ntohs((*iter)->address.sin_port) );
			TRACE("sendto error! (%d)\n", WSAGetLastError());
			assert(0);
		}

		//now add it to our packet free list.
		m_PacketFreeList.push_back((*iter));

		++iter;
	}

	m_OutgoingPackets.clear();

	return true;
}

t_AddressedPacket* NetworkManager::CreatePacket()
{
	//loop the free list first
	if(m_PacketFreeList.size() > 0)
	{
		t_AddressedPacket* pPacket = NULL;

		PacketQueue::iterator iter = m_PacketFreeList.begin();
		if(iter != m_PacketFreeList.end())
		{
			pPacket = (*iter);
			m_PacketFreeList.pop_front();

			memset(pPacket, 0, sizeof(t_AddressedPacket));
			return pPacket;
		}
	}
	else
	{
		TRACE("NEW PACKET\n");

		//None on the free list, so let's NEW one.
		t_AddressedPacket* pPacket = new t_AddressedPacket;
		memset(pPacket, 0, sizeof(t_AddressedPacket));
		return pPacket;
	}

	TRACE("Error obtaining new packet...\n");
	return NULL;
}