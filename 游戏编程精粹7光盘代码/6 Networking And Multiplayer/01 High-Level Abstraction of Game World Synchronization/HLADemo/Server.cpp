#include "StdAfx.h"
#include ".\server.h"
#include "HLADemoDlg.h"
#include "MedivalWorld_S.h"
#include "Client.h"
#include <complex>

using namespace MedivalWorld;

CServer::CServer(CHLADemoDlg	*owner):m_canvas(this),m_HlaRuntime(this)
{
	m_owner=owner;
	m_hostIDGenerator=HLAHostID_None;
	m_pickedKnight=NULL;
	m_pickedViewport=NULL;
}

CServer::~CServer(void)
{
	DestroyAllSynchEntities();
	// purge all clients
	m_remoteClients.clear();
}

void CServer::OnLButtonDown( UINT nFlags, CPoint point )
{
	// action by selected command
	switch(m_canvas.m_owner->m_command)
	{
	case Command_CreateKnight:
		CreateKnight(point);
		break;

	case Command_DestroyKnight:
		DestroyKnight(point);
		break;

	case Command_MoveKnight:
		PickMovableKnight(point);
		break;
	case Command_MoveViewport:
		PickMovableViewport(point);
		break;
	case Command_ChangeKnightState:
		ChangeKnightState(point);
		break;

	}
}

// create a knight object on the position
void CServer::CreateKnight( CPoint point )
{
	RefCount<Knight_S> newOne(new Knight_S);

	newOne->Position=point;
	newOne->IsRed=false;
	newOne->Register(&m_HlaRuntime);

	m_knights.insert(Knights::value_type(newOne->GetInstanceID(),newOne));
}

void CServer::OnRender()
{
	for(Knights::iterator i=m_knights.begin();i!=m_knights.end();i++)
	{
		RefCount<Knight_S> knight=i->second;
		Knight_Render(knight);
	}

	for(RemoteClients::iterator i=m_remoteClients.begin();i!=m_remoteClients.end();i++)
	{
		RefCount<CRemoteClient_S> rc=i->second;
		Viewport_Render(&rc->m_viewport);
	}
}

void CServer::Knight_Render( Knight_S *obj )
{
	Color centerColor(255, 0, 0, 255);
	if(obj->IsRed==true)
		centerColor=Color(255, 255, 0, 0);
	Pen pen(centerColor);
	Graphics& gr=*(m_canvas.m_graphicsOnRender);
	gr.DrawArc(&pen,(int)obj->Position.x-KnightWidth/2,(int)obj->Position.y-KnightWidth/2,KnightWidth,KnightWidth,0.f,360.f);

	CString str;
	str.Format("Knight:%d",obj->GetInstanceID());
	SolidBrush brush(centerColor);
	gr.DrawString(CT2W(str),str.GetLength(),m_canvas.m_font,PointF((float)obj->Position.x,(float)obj->Position.y),&brush);
}

void CServer::Viewport_Render( CDemoSynchViewport *obj )
{
	Color centerColor(255, 90, 90, 90);
	Pen pen(centerColor);
	Graphics& gr=*(m_canvas.m_graphicsOnRender);
	gr.DrawArc(&pen,(int)obj->m_position.x-KnightWidth/2,(int)obj->m_position.y-KnightWidth/2,KnightWidth,KnightWidth,0.f,360.f);
	gr.DrawArc(&pen,(int)obj->m_position.x-ViewportWidth/2,(int)obj->m_position.y-ViewportWidth/2,ViewportWidth,ViewportWidth,0.f,360.f);

	CString str;
	str.Format("Viewport:%d",obj->GetHostID());
	SolidBrush brush(centerColor);
	gr.DrawString(CT2W(str),str.GetLength(),m_canvas.m_font,PointF((float)obj->m_position.x,(float)obj->m_position.y),&brush);
}

void CServer::FrameMove(double elapsedTime)
{
	m_HlaRuntime.FrameMove(elapsedTime);
	m_HlaRuntime.Check_EverySynchEntity_EveryViewport();
}

void CServer::Send( HLAHostID dest,Protocol sendContext,CMessage& message )
{
	CRemoteClient_S* rc=GetRemoteClient(dest);
	if(rc)
	{
		rc->m_virtualConnection->ReceiveQueue_Add(message);
	}
}

CRemoteClient_S* CServer::GetRemoteClient( HLAHostID clientID )
{
	RemoteClients::iterator i=m_remoteClients.find(clientID);
	if(i==m_remoteClients.end())
	{
		return RefCount<CRemoteClient_S>();
	}

	return i->second;
}

/** virtually accept a client and do its event handler */
void CServer::AcceptClient_INTERNAL( CClient* client )
{
	// create a host ID (which is the identified value by server host in real situation)
	client->m_hostID=CreateHostID_INTERNAL();

	// create a remote client object
	RefCount<CRemoteClient_S> newRemoteClient(new CRemoteClient_S(client));	

	// virtual association
	m_remoteClients.insert(RemoteClients::value_type(newRemoteClient->GetHostID(),newRemoteClient));

	// acceptance event handler
	newRemoteClient->m_viewport.Register(&m_HlaRuntime);
}

/** create a host ID.

note that a host ID is an actually network address or a unique identifier. */
HLAHostID CServer::CreateHostID_INTERNAL()
{
	m_hostIDGenerator++;
	return m_hostIDGenerator;
}

// process disconnected client
void CServer::RemoveClient_INTERNAL( CClient* client )
{
	ASSERT(client->m_hostID!=HLAHostID_None);
	
	CRemoteClient_S* rc=GetRemoteClient(client->m_hostID);
	rc->m_viewport.Unregister();
	m_remoteClients.erase(client->m_hostID);
	
	client->m_hostID=HLAHostID_None;
}

float Length(CSize &p)
{
	return sqrt(pow(float(p.cx),2)+pow(float(p.cy),2));
}

bool CServer::IsOneSynchEntityTangibleToOneViewport( SynchEntity_S* entity,SynchViewport_S *viewport )
{
	Knight_S* k=dynamic_cast<Knight_S*>(entity);
	CDemoSynchViewport *v=dynamic_cast<CDemoSynchViewport*>(viewport);

	if(k && v)
	{
		return Length(k->Position-v->m_position)<ViewportWidth/2;
	}
	
	return NULL;
}

// destroy the knight near the point
void CServer::DestroyKnight( CPoint point )
{
	for(Knights::iterator i=m_knights.begin();i!=m_knights.end();i++)
	{
		Knight_S* knight=i->second;		
		if(Length(knight->Position-point) < KnightWidth/2)
		{
			knight->Unregister();
			m_knights.erase(i);
			m_owner->Invalidate();
			return;
		}
	}
}

void CServer::ChangeKnightState( CPoint point )
{
	for(Knights::iterator i=m_knights.begin();i!=m_knights.end();i++)
	{
		Knight_S* knight=i->second;		
		if(Length(knight->Position-point) < KnightWidth/2)
		{
			knight->IsRed=!knight->IsRed;
			m_owner->Invalidate();
			return;
		}
	}
}

void CServer::PickMovableKnight( CPoint point )
{
	for(Knights::iterator i=m_knights.begin();i!=m_knights.end();i++)
	{
		Knight_S* knight=i->second;		
		if(Length(knight->Position-point) < KnightWidth/2)
		{
			m_pickedKnight=knight;
			return;
		}
	}
}

void CServer::PickMovableViewport( CPoint point )
{
	for(RemoteClients::iterator i=m_remoteClients.begin();i!=m_remoteClients.end();i++)
	{
		CDemoSynchViewport* viewport=&i->second->m_viewport;
		if(Length(viewport->m_position-point) < ViewportWidth/2)
		{
			m_pickedViewport=viewport;
			return;
		}
	}
}

void CServer::DestroyAllSynchEntities()
{
	for(Knights::iterator i=m_knights.begin();i!=m_knights.end();i++)	
	{
		i->second->Unregister();
	}
	m_knights.clear();

}

void CServer::OnMouseMove( UINT nFlags, CPoint point )
{
	// move picked knight
	if(m_pickedKnight)
	{
		m_pickedKnight->Position=point;
		m_canvas.Invalidate();
	}

	// move picked viewport
	if(m_pickedViewport)
	{
		m_pickedViewport->m_position=point;
		m_canvas.Invalidate();
	}
}

void CServer::OnLButtonUp( UINT nFlags, CPoint point )
{
	m_pickedKnight=NULL;
	m_pickedViewport=NULL;
}
CRemoteClient_S::CRemoteClient_S(CClient* virtualConnection):m_viewport(virtualConnection->m_hostID)
{
	m_virtualConnection=virtualConnection;
}

HLAHostID CRemoteClient_S::GetHostID()
{
	return m_virtualConnection->m_hostID;
}

CRemoteClient_S::~CRemoteClient_S()
{
	m_viewport.Unregister();
}

CDemoSynchViewport::CDemoSynchViewport(HLAHostID hostID):SynchViewport_S(hostID)
{
	m_position=CPoint((int)ViewportWidth,(int)ViewportWidth);
}