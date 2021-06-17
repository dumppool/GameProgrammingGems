#pragma once

#include "canvas.h"
#include <deque>
#include "../MyHLA/hla.h"
#include "MedivalWorld_C.h"

using namespace std;
using namespace MedivalWorld;

class CServer;
class CHLADemoDlg;

class CClient:public ICanvasDelegate
{
	CServer* m_virtualConnectedServer;

	void ProcessReceivedMessage(CMessage& msg);
	void Entity_Render(SynchEntity_C* entity);
	void Knight_Render(Knight_C* knight);

	CHLADemoDlg* m_owner;
public:
	CCanvas m_canvas;
	HLAHostID m_hostID;
	MedivalWorld::RuntimeDelegate_C m_HlaDg;
	CHlaRuntime_C m_HlaRuntime;

	CClient(CHLADemoDlg	*owner);
	~CClient(void);

	typedef deque<CMessage> MessageQueue;
	MessageQueue m_receivedMessageQueue;

	void OnRender();
	void FrameMove(double elapsedTime);
	void OnLButtonDown(UINT nFlags, CPoint point) {}
	void OnLButtonUp(UINT nFlags, CPoint point) {}
	void OnMouseMove(UINT nFlags, CPoint point) {}
	void ReceiveQueue_Add(CMessage &message);

	bool IsConnectedToServer();
	void Connect(CServer* server);
	void Disconnect();
};
