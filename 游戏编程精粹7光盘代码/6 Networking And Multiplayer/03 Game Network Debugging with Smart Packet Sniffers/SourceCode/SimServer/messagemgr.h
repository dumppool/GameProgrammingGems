#ifndef _MESSAGE_MGR_H_
#define _MESSAGE_MGR_H_

class IListener;
struct t_SysMessage;
struct t_ObjectMessage;

class MessageMgr
{
public:
	MessageMgr();
	~MessageMgr();

	bool Init();
	bool Term();
	bool RegisterListener(IListener* pListener);
	void Update(float fTimeDelta);

	bool SendSystemMessage(t_SysMessage* pMessage);
	bool SendMessageToPlayer(unsigned long nID, t_GameMessage* pMessage, unsigned long nFlags = 0);
	bool DeliverMessage(t_GameMessage* pMessage);
protected:

	typedef std::list<IListener*> ObserverList;
	ObserverList m_Listeners;
};

#endif