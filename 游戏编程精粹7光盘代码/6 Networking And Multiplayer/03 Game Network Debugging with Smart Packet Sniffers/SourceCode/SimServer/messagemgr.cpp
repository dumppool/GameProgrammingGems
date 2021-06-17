#include "StdAfx.h"
#include "MessageMgr.h"
#include "Listener.h"

#include "NetworkManager.h"
extern NetworkManager * gpNetworkMgr;

MessageMgr::MessageMgr()
{

}

MessageMgr::~MessageMgr()
{
	Term();
}

bool MessageMgr::Init()
{
	return false;
}

bool MessageMgr::Term()
{
	return false;
}

void MessageMgr::Update(float fTimeDelta)
{
	UNREFERENCED_PARAMETER(fTimeDelta);
}

bool MessageMgr::RegisterListener(IListener* pListener)
{
	
	//ObserverList::iterator iter 
	 ObserverList::iterator iter = find(m_Listeners.begin(), m_Listeners.end(), pListener);
	 if(iter == m_Listeners.end())
	 {
		 //It's not in our list.. go ahead and add it in.
		 m_Listeners.push_back(pListener);
	 }

	return false;
}

bool MessageMgr::SendSystemMessage(t_SysMessage* pMessage)
{
	ObserverList::iterator iter = m_Listeners.begin();
	while(iter != m_Listeners.end())
	{
		int nResult = (*iter)->OnSysMessage(pMessage);
		if(nResult != 0)
		{
			//This message has been handled.. no need to pass it to other listeners
			return true;
		}

		++iter;
	}

	return false;
}

bool MessageMgr::SendMessageToPlayer(unsigned long nID, t_GameMessage* pMessage, unsigned long nFlags)
{
	if(gpNetworkMgr)
	{
		return gpNetworkMgr->QueueGameMessageForConnection(nID, pMessage, nFlags);
	}

	return false;
}

bool MessageMgr::DeliverMessage(t_GameMessage* pMessage)
{
	ObserverList::iterator iter = m_Listeners.begin();
	while(iter != m_Listeners.end())
	{
		int nResult = (*iter)->OnGameMessage(pMessage);
		if(nResult != 0)
		{
			//this message has been handled.. no need to pass it to any more listeners
			return true;
		}

		++iter;
	}

	return false;
}