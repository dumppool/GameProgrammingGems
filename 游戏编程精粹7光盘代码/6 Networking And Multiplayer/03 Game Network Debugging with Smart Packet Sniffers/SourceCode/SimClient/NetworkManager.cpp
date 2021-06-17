#include "stdafx.h"
#include "NetworkManager.h"
#include "Performance.h"
#include "MessageMgr.h"
#include "NetworkTweakables.h"
extern MessageMgr *gpMessageMgr;

NetworkManager::NetworkManager():
m_eState(NET_STATE_IDLE),
m_Socket(INVALID_SOCKET),
m_fBrowseTime(0.0f)
{
	//memset(&m_Connection, 0, sizeof(t_Connection));
}

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

	//memset(&m_Connection, 0, sizeof(t_Connection));

	// Initialize our socket
	InitSocket();

	return true;
}

bool NetworkManager::Term()
{
	m_eState = NET_STATE_IDLE;

	//Clear our freelist
	PacketQueue::iterator iter = m_PacketFreeList.begin();
	while(iter != m_PacketFreeList.end())
	{
		delete (*iter);
		++iter;
	}

	ServerInfoList::iterator iterServers = m_ServerInfoList.begin();
	while(iterServers != m_ServerInfoList.end())
	{
		delete (*iterServers);

		++iterServers;
	}

	//FIXME: check for an error code here...
	WSACleanup();

	return false;
}

bool NetworkManager::InitSocket()
{
	//create the socket
	m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_Socket == INVALID_SOCKET)
	{
		TRACE("socket() failed. (%d)\n", WSAGetLastError());	
		return false;
	}

	//set sockopt

	int x = 1;
	/*
	if (setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (char *)&x, sizeof(x))== SOCKET_ERROR) 
	{
		TRACE("setsockopt failed. (%d)\n", WSAGetLastError());	
		return false;
	}
	*/

	//set non-blocking
	u_long iMode = 1;
	if(ioctlsocket(m_Socket, FIONBIO, &iMode) == SOCKET_ERROR)
	{
		TRACE("ioctlsocket failed. (%d)\n", WSAGetLastError());	
		return false;
	}


	if (setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST, (char *)&x, sizeof(x))== SOCKET_ERROR) 
	{
		TRACE("setsockopt failed. (%d)\n", WSAGetLastError());	
		return false;
	}

	//bind the socket
	/* Set family and port */
	struct sockaddr_in service;
	memset((void *)&service, '\0', sizeof(struct sockaddr_in));
	service.sin_family = AF_INET;
	service.sin_port = htons(0);
	service.sin_addr.s_addr = INADDR_ANY;
	if (bind(m_Socket, (struct sockaddr *)&service, sizeof(struct sockaddr_in)) == -1)
	{
		TRACE("Could not bind name to socket. (%d)\n", WSAGetLastError());					
		return false;
	}

	return true;
}

bool NetworkManager::CloseSocket()
{
	if(m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}

	return true;
}

void NetworkManager::Update(float fTimeDelta)
{
	if(m_eState == NET_STATE_IDLE)
	{
		//Don't do anything...
		return;
	}

	if(m_eState == NET_STATE_BROWSING)
	{
		TRACE("UpdateBrowseLAN\n");
		UpdateBrowseLAN(fTimeDelta);
	}

	if(m_eState == NET_STATE_BROWSING_COMPLETE)
	{
		//try to connect now...
		TRACE("NET_STATE_BROWSING_COMPLETE\n");

		ServerInfoList::iterator iterServers = m_ServerInfoList.begin();
		if(iterServers != m_ServerInfoList.end())
		{
			TRACE("Connect...\n");
			//just connect to the first one in our list.
			Connect( (*iterServers)->address );
		}
		else
		{
			m_eState = NET_STATE_IDLE;
		}
	}

	CPerformanceCounter perfCounter;
	perfCounter.Start();

	bool bContinueLoop = true;
	while(bContinueLoop)
	{
		bool bReceived = HandleIncomingPacket();
		if(!bReceived)
		{
			bContinueLoop = false;
			continue;
		}

		perfCounter.Stop();
		//if(perfCounter.TimeInMilliseconds() > 16.67f)
		if(perfCounter.TimeInMilliseconds() > 66.66f)
		{
			//Jump out
			bContinueLoop = false;
			continue;
		}
	}

	// Update our connection
	UpdateConnection(fTimeDelta);

	// Flush outgoing packets...
	FlushOutgoingPackets();
}

bool NetworkManager::Connect(const char* szAddress, unsigned short port)
{
	if((szAddress == NULL) || (szAddress[0] == '\0') || (port == 0))
	{
		return false;
	}

	//TODO: fill t_Address struct with this data
	struct sockaddr_in address;
	memset(&address, 0, sizeof(sockaddr_in));
	address.sin_family		= AF_INET;
	address.sin_addr.s_addr = inet_addr(szAddress);
	address.sin_port		= htons(port);

	return Connect(address);
}

bool NetworkManager::Connect(const struct sockaddr_in &address)
{
	TRACE("Connecting to : %s:%d\n", inet_ntoa(address.sin_addr), address.sin_port );

	m_Connection.lastJoinPacketSent = 2.0f;
	memcpy(&m_Connection.address, &address, sizeof(struct sockaddr_in));

	m_eState = NET_STATE_CONNECTING;
	return false;
}

bool NetworkManager::BrowseLAN()
{
	m_eState = NET_STATE_BROWSING;
	m_fBrowseTime = 0.0f;

	//clear out old hosts
	ServerInfoList::iterator iterServers = m_ServerInfoList.begin();
	while(iterServers != m_ServerInfoList.end())
	{
		delete (*iterServers);
		++iterServers;
	}

	//send browse packet
	t_AddressedPacket* pPacket = CreatePacket();
	if(pPacket)
	{
		pPacket->SetType(MSG_BROWSE);
		pPacket->address.sin_port = htons(8500);
		pPacket->payload.header.flags = MSG_FLAG_BROADCAST;
		QueuePacket(pPacket);
	}

	return false;
}

bool NetworkManager::UpdateBrowseLAN(float fTimeDelta)
{
	//are we at the timeout?
	m_fBrowseTime += fTimeDelta;
	if(m_fBrowseTime >= NetworkTweakables::GetInstance().GetBrowseTimeout())
	{
		m_eState = NET_STATE_BROWSING_COMPLETE;
	}

	return false;
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
		//None on the free list, so let's NEW one.
		t_AddressedPacket* pPacket = new t_AddressedPacket;
		memset(pPacket, 0, sizeof(t_AddressedPacket));
		return pPacket;
	}

	TRACE("Error obtaining new packet...\n");
	return NULL;
}

bool NetworkManager::UpdateConnection(float fTimeDelta)
{
	UNREFERENCED_PARAMETER(fTimeDelta);

	switch(m_eState)
	{
	case NET_STATE_CONNECTING:
		{
			m_Connection.lastJoinPacketSent += fTimeDelta;
			if(m_Connection.lastJoinPacketSent > 2.0f)
			{
				m_Connection.lastJoinPacketSent = 0;
				//QUEUE a new packet...
				t_AddressedPacket* pPacket = CreatePacket();
				if(pPacket != NULL)
				{
					memcpy(&pPacket->address, &m_Connection.address, sizeof(struct sockaddr_in));
					pPacket->payload.SetType(MSG_REQUEST_JOIN);
					pPacket->payload.msgJoinRequest.nVersion = PROTOCOL_VERSION;
					QueuePacket(pPacket);
				}

			}
		}break;
	case NET_STATE_CONNECTED:
		{
			m_Connection.timeDeltaCounter += fTimeDelta;
			if(m_Connection.timeDeltaCounter >= 5.0f)
			{
				//DROP
				TRACE("No heartbeat from connection in 5 seconds.... stop listening...\n");
				m_eState = NET_STATE_IDLE;
			}

			m_Connection.lastHeartbeatSent += fTimeDelta;
			if(m_Connection.lastHeartbeatSent >= 1.0f)
			{
				m_Connection.lastHeartbeatSent = 0;

				t_AddressedPacket* pPacket = CreatePacket();
				if(pPacket != NULL)
				{
					TRACE("Sending heartbeat");

					memcpy(&pPacket->address, &m_Connection.address, sizeof(struct sockaddr_in));
					pPacket->payload.SetType(MSG_GAME_HEARTBEAT);
					pPacket->payload.msgHeartbeat.nReserved = 0;
					QueuePacket(pPacket);
				}
			}

		}break;
	default:
		break;
	}

	return false;
}

bool NetworkManager::HandleIncomingPacket()
{
	//read from receiver
	struct sockaddr_in client;
	int client_length = (int)sizeof(struct sockaddr_in);
	memset(&client, 0, client_length);

	t_AddressedPacket packet;
	memset(&packet.payload, 0, sizeof(t_Packet));

	int nBytesReceived = recvfrom(	m_Socket, 
									(char*)&packet.payload, 
									packet.payload.GetSize(), 
									0, 
									(struct sockaddr *)&packet.address, 
									&client_length);

	if(nBytesReceived == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if(err != WSAEWOULDBLOCK)
		{
			TRACE("recvfrom error! (%d)\n", WSAGetLastError());
		}
		return false;			
	}

	if(nBytesReceived == 0)
	{
		// There's no data on the wire.
		return false;
	}

	if(!ValidatePacket(packet, nBytesReceived))
	{
		return false;
	}

	TRACE("Incomming %d packet\n", packet.payload.GetType());

	//pass it to a handler...
	switch( packet.payload.GetType() )
	{
	case MSG_REQUEST_JOIN_RESPONSE:
		{
			return HandleJoinResponse(packet);
		}
	case MSG_GAME_HEARTBEAT:
		{
			return HandleHeartbeat(packet);
		}
	case MSG_GAME_DATA:
		{
			return HandleGameData(packet);
		}
	case MSG_BROWSE_RESPONSE:
		{
			return HandleBrowseResponse(packet);
		}
	default:
		break;
	}

	return false;
}

bool NetworkManager::HandleJoinResponse(t_AddressedPacket& packet)
{
	if(m_eState == NET_STATE_CONNECTING)
	{
		if(packet.payload.GetType() == MSG_REQUEST_JOIN_RESPONSE)
		{
			if(packet.payload.msgJoinResponse.eCode != JOIN_CODE_OK)
			{
				TRACE("Server denied our request to join...\n");
				m_eState = NET_STATE_IDLE;
				return true;
			}

			if(packet.payload.msgJoinResponse.nVersion != PROTOCOL_VERSION)
			{
				TRACE("Server protocol version differs from ours...\n");

				m_eState = NET_STATE_IDLE;
				return true;
			}

			//Send a join ack
			t_AddressedPacket* pPacket = CreatePacket();
			if(pPacket != NULL)
			{
				memcpy(&pPacket->address, &m_Connection.address, sizeof(struct sockaddr_in));
				pPacket->payload.SetType(MSG_REQUEST_JOIN_RESPONSE_ACK);
				pPacket->payload.msgJoinResponseAck.nReserved = 0;
				QueuePacket(pPacket);
				m_eState = NET_STATE_CONNECTED;

				return true;
			}
		}

		return false;
	}

	return false;
}

bool NetworkManager::HandleHeartbeat(t_AddressedPacket& packet)
{
	UNREFERENCED_PARAMETER(packet);
	m_Connection.timeDeltaCounter = 0;

	return true;
}

bool NetworkManager::HandleGameData(t_AddressedPacket& packet)
{
	if(packet.payload.header.flags & MSG_FLAG_RELIABLE )
	{
		TRACE("Send ACK!\n");
		//send an ack
		t_AddressedPacket* pPacket = CreatePacket();
		memcpy(&pPacket->address, &m_Connection.address, sizeof(struct sockaddr_in));

		pPacket->SetType(MSG_RELIABLE_ACK);
		pPacket->payload.msgAck.nSequenceNumber = packet.payload.header.sequenceNum;
		QueuePacket(pPacket);
	}

	return gpMessageMgr->DeliverMessage(&packet.payload.msgGame);
}

bool NetworkManager::HandleBrowseResponse(t_AddressedPacket& packet)
{
	//first check the version
	if(packet.payload.msgBrowseResponse.nVersion != PROTOCOL_VERSION)
	{
		//ignore this server if we're not using the same version.
		TRACE("invalid version\n");
		return false;
	}


	//add this host to our list of hosts that responded
	//make sure it's not already in our list
	ServerInfoList::iterator iter = m_ServerInfoList.begin();
	while(iter != m_ServerInfoList.end())
	{
		if( memcmp( &(packet.address), &((*iter)->address), sizeof(sockaddr_in)) != 0)
		{
			TRACE("this is in our list\n");
			return false;
		}

		++iter;
	}

	ServerInfo* pInfo = new ServerInfo;
	if(pInfo)
	{
		memcpy( &pInfo->address, &((packet).address), sizeof(sockaddr_in));
		pInfo->nNumPlayers = packet.payload.msgBrowseResponse.nNumPlayers;

		memset(pInfo->szGameDesc, 0, 17);
		strncpy(pInfo->szGameDesc, packet.payload.msgBrowseResponse.szGameDesc, 16);

		m_ServerInfoList.push_back(pInfo);

		TRACE("pushback this server\n");
		return true;
	}

	TRACE("nothing\n");
	return false;
}

bool NetworkManager::QueuePacket(t_AddressedPacket* packet)
{
	m_OutgoingPacketQueue.push_back(packet);

	return false;
}



bool NetworkManager::FlushOutgoingPackets()
{
	PacketQueue::iterator iter = m_OutgoingPacketQueue.begin();
	while(iter != m_OutgoingPacketQueue.end())
	{
		TRACE("Outgoing packet (%d)\n", (*iter)->payload.GetType());

		if( (*iter)->payload.header.flags & MSG_FLAG_BROADCAST )
		{
			(*iter)->address.sin_family = AF_INET;
			(*iter)->address.sin_addr.s_addr = INADDR_BROADCAST;
		}

		// Create a checksum for this packet.
		ChecksumPacket( (t_Packet*)&((*iter)->payload) );

		//make it fail...
		//(*iter)->payload.GetHeader().crc = 0xff4455ff;

		//send it
		int nNumBytes = sendto(m_Socket, 
								(char*)&((*iter)->payload), 
								(*iter)->payload.GetSize(), 
								0, 
								(struct sockaddr*)&((*iter)->address), 
								sizeof(struct sockaddr_in));

		if( (nNumBytes == SOCKET_ERROR) || (nNumBytes == 0) )
		{
			TRACE("client: %s:%d\n", inet_ntoa((*iter)->address.sin_addr), ntohs((*iter)->address.sin_port) );
			TRACE("sendto error! (%d)\n", WSAGetLastError());
			return false;
		}

		m_PacketFreeList.push_back( (*iter) );

		++iter;
	}

	m_OutgoingPacketQueue.clear();

	return false;
}

bool NetworkManager::ValidatePacket(t_AddressedPacket& packet, int nSize)
{
	//checksum validation
	if( !ValidateChecksum((t_Packet*)&(packet.payload)) )
	{
		TRACE("failed checksum\n");
		return false;
	}

	// Validate this packet address as our server.
	if(!ValidateAddress(packet.address) && (packet.GetType() != MSG_BROWSE_RESPONSE) )
	{
		TRACE("invalid address\n");
		return false;
	}

	//parse for type
	//check for size validation
	if(nSize != packet.payload.GetSize())
	{
		TRACE("wrong payload size\n");
		return false;
	}

	return true;
}

bool NetworkManager::ValidateAddress(const struct sockaddr_in &address)
{
	if(memcmp(&m_Connection.address, &address, sizeof(struct sockaddr_in)) != 0)
	{
		TRACE("Warning: packet incoming from invalid address...\n");
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
	if( CreateChecksum((unsigned char*)packet, packet->GetSize()) == crc )
	{
		packet->header.crc = crc;
		return true;
	}

	return false;
}

//FIXME: this is a quick hack
bool NetworkManager::QueueMessageToServer(t_GameMessage* pMessage)
{
	t_AddressedPacket* pPacket = CreatePacket();
	memset(pPacket, 0, sizeof(t_Packet));
	memcpy(&pPacket->address, &m_Connection.address, sizeof(struct sockaddr_in));

	pPacket->payload.header.nMsgID = MSG_GAME_DATA;

	//pPacket->payload.msgGame.nMsgID = GAME_MSG_PLAYER_INFO;
	memcpy(&pPacket->payload.msgGame, pMessage, sizeof(t_GameMessage));
	QueuePacket(pPacket);

	return false;
}