#ifndef _NETWORK_PROTOCOL_H_
#define _NETWORK_PROTOCOL_H_

#include "Message.h"

#define PROTOCOL_VERSION 3

enum EMessageType
{
	MSG_NONE = 0,
	MSG_REQUEST_JOIN,
	MSG_REQUEST_JOIN_RESPONSE,
	MSG_REQUEST_JOIN_RESPONSE_ACK,
	MSG_GAME_HEARTBEAT,
	MSG_GAME_ENTITYINFO,
	MSG_GAME_DISCONNECT,
	MSG_GAME_DATA,
	MSG_RELIABLE_ACK,
	MSG_BROWSE,
	MSG_BROWSE_RESPONSE,
	MSG_LAST,
	MSG_FORCE32BITS = 0xFFFFFFFF
};

enum EJoinCode
{
	JOIN_CODE_OK = 0,
	JOIN_CODE_DENIED,
	JOIN_CODE_FORCE32BITS = 0xFFFFFFFF
};

#define MSG_FLAG_RELIABLE (1<<0)
#define MSG_FLAG_BROADCAST (1<<1)

#pragma pack(push, 1)
struct t_PacketHeader
{
	EMessageType	nMsgID;
	unsigned long	sequenceNum;
	unsigned long	crc;
	unsigned long	timestamp;
	unsigned long	flags;
};

// Assume these structures are filled with relevant variables.
struct t_MessageJoinRequest
{
	unsigned short nVersion;
};

struct t_MessageJoinResponse
{
	EJoinCode eCode;
	unsigned short nVersion;
};

struct t_MessageJoinResponseAck
{
	unsigned long nReserved;
};

struct t_MessageHeartbeat
{
	unsigned long nReserved;
};

struct t_MessageEntityInfo
{
	unsigned long nReserved;
};

struct t_MessageDisconnect
{
	unsigned long nReserved;
};

struct t_MessageReliableAck
{
	unsigned long nSequenceNumber;
};

struct t_MessageBrowse
{
	unsigned long nNumber;
};

struct t_MessageBrowseResponse
{
	unsigned short nVersion;
	unsigned short nNumPlayers;
	char	szGameDesc[16];
};


class t_Packet
{
public:
	
	EMessageType GetType()
	{
		return header.nMsgID;
	}

	void SetType(EMessageType eType)
	{
		header.nMsgID = eType;
	}

	bool IsOOB()
	{
		switch(header.nMsgID)
		{
		case MSG_REQUEST_JOIN:
		case MSG_REQUEST_JOIN_RESPONSE: //this one is really only handled client side.
		case MSG_REQUEST_JOIN_RESPONSE_ACK:
		case MSG_BROWSE:
		case MSG_BROWSE_RESPONSE:
			return true;

		default:
			return false;
		}
	}

	unsigned short GetSize()
	{
		unsigned short nSize = sizeof(t_PacketHeader);

		switch(header.nMsgID)
		{
		case MSG_NONE:
			return sizeof(t_Packet);
		case MSG_REQUEST_JOIN:
			nSize += sizeof(t_MessageJoinRequest);
			break;
		case MSG_REQUEST_JOIN_RESPONSE:
			nSize += sizeof(t_MessageJoinResponse);
			break;
		case MSG_REQUEST_JOIN_RESPONSE_ACK:
			nSize += sizeof(t_MessageJoinResponseAck);
			break;
		case MSG_GAME_HEARTBEAT:
			nSize += sizeof(t_MessageHeartbeat);
			break;
		case MSG_GAME_ENTITYINFO:
			nSize += sizeof(t_MessageEntityInfo);
			break;
		case MSG_GAME_DISCONNECT:
			nSize += sizeof(t_MessageDisconnect);
			break;
		case MSG_GAME_DATA:
			nSize += sizeof(t_GameMessage);
			break;
		case MSG_RELIABLE_ACK:
			nSize += sizeof(t_MessageReliableAck);
			break;
		case MSG_BROWSE:
			nSize += sizeof(t_MessageBrowse);
			break;
		case MSG_BROWSE_RESPONSE:
			nSize += sizeof(t_MessageBrowseResponse);
			break;
		default:
			{
				assert(!"Invalid Message Type!");
				return 0;
			}
		}

		return nSize;
	}

	t_PacketHeader& GetHeader() { return header; }

	t_PacketHeader header;
	union
	{
		t_MessageJoinRequest	 	msgJoinRequest;
		t_MessageJoinResponse 		msgJoinResponse;
		t_MessageJoinResponseAck 	msgJoinResponseAck;
		t_MessageHeartbeat 			msgHeartbeat;
		t_MessageEntityInfo 		msgEntityInfo;
		t_MessageDisconnect 		msgDisconnect;
		t_GameMessage				msgGame;
		t_MessageReliableAck		msgAck;
		t_MessageBrowse				msgBrowse;
		t_MessageBrowseResponse		msgBrowseResponse;
	};
};
#pragma pack(pop)

class t_AddressedPacket
{
public:
	struct sockaddr_in address;
	t_Packet payload;
	float fResendTimeout;

	//if we've resent this some certain number of times, without an ack
	//perhaps we need to do something interesting... (boot the player, who knows)
	unsigned long nResendCount;

	unsigned short GetPayloadSize()
	{
		return payload.GetSize();
	}

	EMessageType GetType()
	{
		return payload.GetType();
	}

	void SetType(EMessageType eType)
	{
		return payload.SetType(eType);
	}

	bool IsOOB()
	{
		return payload.IsOOB();
	}
};
typedef std::list<t_AddressedPacket*> PacketQueue;

#endif
