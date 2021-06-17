#ifndef _MESSAGE_MGR_H_
#define _MESSAGE_MGR_H_

class IListener;
struct t_GameMessage;

class MessageMgr
{
public:
	MessageMgr();
	~MessageMgr();

	bool Init();
	bool Term();
	bool RegisterListener(IListener* pListener);
	void Update(float fTimeDelta);

	bool DeliverMessage(t_GameMessage* pMessage);
	bool SendMessageToServer(t_GameMessage* pMessage);
protected:

	//We'll fill this in later...
	//list of messages
	//list of listener pointers
	typedef std::list<IListener*> ObserverList;
	ObserverList m_Listeners;
};

#endif