#ifndef _LISTENER_H_
#define _LISTENER_H_

struct t_GameMessage;
struct t_SysMessage;

class IListener
{
public:
	virtual int OnSysMessage(t_SysMessage* pMessage) = 0;
	virtual int OnGameMessage(t_GameMessage* pMessage) = 0;
};

#endif