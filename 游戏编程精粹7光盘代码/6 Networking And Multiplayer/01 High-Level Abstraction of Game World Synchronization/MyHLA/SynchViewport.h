#pragma once

#include <set>
#include "Message.h"
#include "ptr.h"

class SynchEntity_S;
class CHlaRuntime_S;

class SynchViewport_S
{
public:
	typedef std::set<SynchEntity_S*> SynchEntityList;
	SynchEntityList m_tangibles_INTERNAL;
private:
	HLAHostID m_hostID;
	CHlaRuntime_S* m_runtime;
public:
	SynchViewport_S(HLAHostID hostID);
	virtual ~SynchViewport_S(void);

	bool AddTangible_INTERNAL(SynchEntity_S* entity);
	bool RemoveTangible_INTERNAL(SynchEntity_S* entity);

	void Register(CHlaRuntime_S* runtime);
	void Unregister();

	inline HLAInstanceID GetHostID() { return m_hostID; }
	bool IsRegistered() { return m_runtime!=NULL; }
};
