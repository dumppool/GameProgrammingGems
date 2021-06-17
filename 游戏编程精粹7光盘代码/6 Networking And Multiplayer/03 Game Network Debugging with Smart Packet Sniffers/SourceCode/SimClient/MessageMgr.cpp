#include "StdAfx.h"
#include "MessageMgr.h"
#include "Listener.h"
#include "NetworkManager.h"

extern NetworkManager * gpNetworkManager;

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
	m_Listeners.clear();

	return false;
}

void MessageMgr::Update(float fTimeDelta)
{
	UNREFERENCED_PARAMETER(fTimeDelta);
}

bool MessageMgr::RegisterListener(IListener* pListener)
{
	ObserverList::iterator iter = find(m_Listeners.begin(), m_Listeners.end(), pListener);
	if(iter == m_Listeners.end())
	{
		m_Listeners.push_back(pListener);
		return true;
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

bool MessageMgr::SendMessageToServer(t_GameMessage* pMessage)
{
	if(pMessage == NULL)
	{
		return false;
	}

	return gpNetworkManager->QueueMessageToServer(pMessage);
}