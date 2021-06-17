#pragma once

#include "canvas.h"
#include "../MyHLA/Hla.h"
#include "MedivalWorld_S.h"

using namespace MedivalWorld;

class CClient;
class CHLADemoDlg;

typedef map<HLAInstanceID,RefCount<Knight_S> > Knights;

class CDemoSynchViewport:public SynchViewport_S
{
public:
	CPoint m_position;

	CDemoSynchViewport(HLAHostID hostID);
};

/** remote client instance for an accepted connection at the server side */
class CRemoteClient_S
{
public:
	CDemoSynchViewport m_viewport;

	CClient* m_virtualConnection; // virtual network connection
	CRemoteClient_S(CClient* virtualConnection);
	~CRemoteClient_S();

	HLAHostID GetHostID();
};

class CServer:public ICanvasDelegate,public IHlaRuntimeDelegate_S
{
	HLAHostID m_hostIDGenerator;
	CHLADemoDlg *m_owner;

	void DestroyAllSynchEntities();

	Knight_S* m_pickedKnight;
	CDemoSynchViewport* m_pickedViewport;
public:
	CCanvas m_canvas;

	// HLA runtime object
	CHlaRuntime_S m_HlaRuntime;
	
	Knights m_knights;

	// virtual network client
	typedef map<HLAHostID,RefCount<CRemoteClient_S> > RemoteClients;
	RemoteClients m_remoteClients;

	CServer(CHLADemoDlg	*owner);
	~CServer(void);

	void OnRender();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);

	void CreateKnight(CPoint point);
	void ChangeKnightState( CPoint point );
	void DestroyKnight(CPoint point);
	void PickMovableKnight( CPoint point );
	void PickMovableViewport( CPoint point );

	void FrameMove(double elapsedTime);
	void Knight_Render(Knight_S *obj);
	void Viewport_Render( CDemoSynchViewport *obj );

	virtual void Send(HLAHostID dest,Protocol sendContext,CMessage& message);

	CRemoteClient_S* GetRemoteClient(HLAHostID clientID);

	void AcceptClient_INTERNAL(CClient* client);
	void RemoveClient_INTERNAL(CClient* client);
	HLAHostID CreateHostID_INTERNAL();

	virtual bool IsOneSynchEntityTangibleToOneViewport(SynchEntity_S* entity,SynchViewport_S *viewport);
};
