#include "StdAfx.h"
#include ".\client.h"
#include "Server.h"
#include "HLADemoDlg.h"

CClient::CClient(CHLADemoDlg *owner):m_canvas(this)
{
	m_owner=owner;
	m_hostID=HLAHostID_None;
	m_virtualConnectedServer=NULL;

	m_HlaRuntime.AddDelegate(&m_HlaDg);
}

CClient::~CClient(void)
{
	Disconnect();
}

void CClient::ReceiveQueue_Add( CMessage &message )
{
	m_receivedMessageQueue.push_back(message);
}

bool CClient::IsConnectedToServer()
{
	return m_virtualConnectedServer!=NULL;
}

// virtually connect to server
void CClient::Connect(CServer* server)
{
	m_virtualConnectedServer=server;
	server->AcceptClient_INTERNAL(this);
}

void CClient::Disconnect()
{
	if(m_virtualConnectedServer!=NULL)
	{
		m_virtualConnectedServer->RemoveClient_INTERNAL(this);
		m_virtualConnectedServer=NULL;
		m_HlaRuntime.Clear();
	}
}

void CClient::FrameMove( double elapsedTime )
{
	// process received messages
	while(m_receivedMessageQueue.empty()==false)
	{
		CMessage& msg=m_receivedMessageQueue.front();
		ProcessReceivedMessage(msg);
		m_receivedMessageQueue.pop_front();
	}
}

void CClient::ProcessReceivedMessage( CMessage& msg )
{
	msg.SetCursor(0);  // to make sure that the reading point is reset
	m_HlaRuntime.ProcessReceivedMessage(msg);
	CString t=m_HlaRuntime.GetLastProcessMessageReport();
	m_owner->AddMessageLog(t);
}

void CClient::OnRender()
{
	for(SynchEntities_C::iterator i=m_HlaRuntime.m_synchEntities.begin();i!=m_HlaRuntime.m_synchEntities.end();i++)
	{
		SynchEntity_C* entity=i->second;
		Entity_Render(entity);
	}
}

void CClient::Entity_Render( SynchEntity_C* entity )
{
	if(Knight_C* knight=dynamic_cast<Knight_C*>(entity))
	{
		Knight_Render(knight);
	}
}

void CClient::Knight_Render( Knight_C* knight )
{
	Color centerColor(255, 0, 0, 255);
	if(knight->IsRed==true)
		centerColor=Color(255, 255, 0, 0);
	Pen pen(centerColor);
	Graphics& gr=*(m_canvas.m_graphicsOnRender);
	gr.DrawArc(&pen,(int)knight->Position.x-KnightWidth/2,(int)knight->Position.y-KnightWidth/2,KnightWidth,KnightWidth,0.f,360.f);

	CString str;
	str.Format("Knight:%d",knight->GetInstanceID());
	SolidBrush brush(centerColor);
	gr.DrawString(CT2W(str),str.GetLength(),m_canvas.m_font,PointF((float)knight->Position.x,(float)knight->Position.y),&brush);
}