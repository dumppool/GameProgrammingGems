#pragma once

#include "message.h"
#include "ptr.h"

class CHlaRuntime_S;
class SynchEntity_S;
class SynchViewport_S;

/** The base class of HLA synchronized entities
*/
class SynchEntity_S
{
public:
	typedef std::set<SynchViewport_S*> SynchViewportList;
	SynchViewportList m_beholders_INTERNAL;
private:
	CHlaRuntime_S *m_runtime;
	HLAInstanceID m_instanceID;
public:
	SynchEntity_S(void);
	virtual ~SynchEntity_S(void);

	void Register(CHlaRuntime_S *runtime);
	void Unregister();	

	void CreateAppearMessageOnNeed_INTERNAL(CMessage& output);
	void CreateDisappearMessageOnNeed_INTERNAL( CMessage& output );

	inline HLAInstanceID GetInstanceID() { return m_instanceID; }

	virtual bool GatherTheChangeToMessage( CMessage &outputMessage,Protocol &outProtocol )=0;
	virtual void GatherAllToMessage( CMessage &outputMessage )=0;
	virtual HLAClassID GetClassID()=0;
	virtual bool HasChanges()=0;
	virtual void ClearTheChanges()=0;

	bool IsRegistered() { return m_runtime!=NULL; }
	void GetViewportOwnerHostList(vector<HLAHostID> &outHostList);
};

/** client-side object of SynchEntity

Behaviors
- This class is actually the replica of SynchEntity_S object considering the viewport culling.
*/
class SynchEntity_C
{
public:
	HLAInstanceID m_instanceID_INTERNAL;

	virtual ~SynchEntity_C();

	virtual void UpdateFromMessage(CMessage& msg)=0;
	inline HLAInstanceID GetInstanceID() { return m_instanceID_INTERNAL; }
};