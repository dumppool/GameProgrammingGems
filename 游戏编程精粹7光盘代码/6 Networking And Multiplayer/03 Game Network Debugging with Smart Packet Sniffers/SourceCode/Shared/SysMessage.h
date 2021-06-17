#ifndef _SYS_MESSAGE_H_
#define _SYS_MESSAGE_H_

enum ESysMessageType
{
	SYS_MSG_NONE = 0,
	SYS_MSG_PLAYER_CONNECTION,
	SYS_MSG_FORCE32BITS = 0xFFFFFFFF
};

enum ESysPlayerConnection
{
	PLAYER_CONNECTED = 0,
	PLAYER_DISCONNECTED,
	PLAYER__CONNECTION_FORCE32BITS = 0xFFFFFFFF
};

struct t_SysMessagePlayerConnection
{
	unsigned long			nID;	//connection ID
	ESysPlayerConnection	eCode;
};


#pragma pack(push, 1)
struct t_SysMessage
{
	ESysMessageType	nMsgID;

	union 
	{
		t_SysMessagePlayerConnection sysMessagePlayerConnection;
	};
};
#pragma pack(pop)

#endif