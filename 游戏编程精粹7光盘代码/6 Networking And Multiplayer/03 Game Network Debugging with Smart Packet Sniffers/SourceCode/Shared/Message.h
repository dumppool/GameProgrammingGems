#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum EGameMessageType
{
	GAME_MSG_NONE = 0,
	GAME_MSG_PLAYER_INFO,		//this is info incoming to the server
	GAME_MSG_PLAYER_START_INFO, //this is sent only to one player
	GAME_MSG_ENTITY_CREATE,
	GAME_MSG_ENTITY_UPDATE,
	GAME_MSG_ENTITY_DESTROY,
	GAME_MSG_FORCE32BITS = 0xFFFFFFFF
};

struct NetFloat3
{
	//TODO: we're going to want to do endian swapping.
	float x;
	float y;
	float z;
};

struct t_GameMessagePlayerInfo
{
	//These variables will be expanded upon as we progress.	
	unsigned long	nID;
	NetFloat3		vPos;
	float			fFacing;
	unsigned long	nCommands;
};

struct t_GameMessagePlayerStartInfo
{
	unsigned long	nID;		//unique objectID
	NetFloat3		vPos;		//starting pos
	float			fFacing;	//initial facing angle
};

struct t_GameMessageEntityCreate
{
	unsigned long	nID;
	NetFloat3		vPos;
	NetFloat3		vDir;
	float			fFacing;
	unsigned int	nType;
	unsigned long	nCommands;
};

struct t_GameMessageEntityUpdate
{
	unsigned long	nID;
	NetFloat3		vPos;
	NetFloat3		vDir;
	float			fFacing;
	unsigned int	nType;
	unsigned long	nCommands;
};

struct t_GameMessageEntityDestroy
{
	unsigned long	nID;
};

#pragma pack(push, 1)
struct t_GameMessage
{
	EGameMessageType	nMsgID;
	unsigned long		nID; //unique player ID

	union 
	{
		t_GameMessagePlayerInfo			gameMessagePlayerInfo;
		t_GameMessagePlayerStartInfo	gameMessagePlayerStartInfo;
		t_GameMessageEntityCreate		gameMessageEntityCreate;
		t_GameMessageEntityUpdate		gameMessageEntityUpdate;
		t_GameMessageEntityDestroy		gameMessageEntityDestroy;
	};
};
#pragma pack(pop)

#endif