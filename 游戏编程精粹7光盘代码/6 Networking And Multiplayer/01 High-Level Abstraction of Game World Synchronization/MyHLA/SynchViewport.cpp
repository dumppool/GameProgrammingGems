#include "StdAfx.h"
#include ".\synchviewport.h"
#include "SynchEntity.h"
#include "HlaRuntime.h"

SynchViewport_S::SynchViewport_S(HLAHostID hostID)
{
	m_runtime=NULL;
	m_hostID=hostID;
}

SynchViewport_S::~SynchViewport_S(void)
{
	if(IsRegistered()==true)
		throw "SynchViewport must have been unregistered before its destructor is called";

}

/** \return true if newly added */
bool SynchViewport_S::AddTangible_INTERNAL( SynchEntity_S* entity )
{
	return (m_tangibles_INTERNAL.insert(entity).second==true);
}

/** \return true if newly removed */
bool SynchViewport_S::RemoveTangible_INTERNAL( SynchEntity_S* entity )
{
	if(m_tangibles_INTERNAL.find(entity)!=m_tangibles_INTERNAL.end())
	{
		m_tangibles_INTERNAL.erase(entity);
		return true;
	}

	return false;
}

void SynchViewport_S::Register( CHlaRuntime_S* runtime )
{
	// verification
	if(GetHostID()==HLAInstanceID_None)
		throw "host ID must be set before registration!";

	if(m_runtime!=runtime)
	{
		Unregister();

		// verification
		if(runtime->m_synchViewports.find(GetHostID())!=runtime->m_synchViewports.end())
		{
			throw "duplicated host ID during registration!";
		}

		// commit 
		runtime->m_synchViewports.insert(SynchViewports_S::value_type(GetHostID(),this));
		m_runtime=runtime;

		m_runtime->Check_EverySynchEntity_OneViewport(this);
	}
}

void SynchViewport_S::Unregister()
{
	if(m_runtime!=NULL)
	{
		m_runtime->RemoveOneSynchViewport_INTERNAL(this);
		m_runtime->m_synchViewports.erase(GetHostID());
		m_runtime=NULL;
	}
}
