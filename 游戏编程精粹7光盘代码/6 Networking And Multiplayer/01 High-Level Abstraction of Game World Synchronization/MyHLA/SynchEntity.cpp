#include "StdAfx.h"
#include ".\synchentity.h"
#include "HlaRuntime.h"
#include "SynchViewport.h"

SynchEntity_S::SynchEntity_S(void)
{
	m_runtime=NULL;
	m_instanceID=HLAInstanceID_None;
}

SynchEntity_S::~SynchEntity_S(void)
{
	if(IsRegistered()==true)
		throw "SynchEntity must have been unregistered before its destructor is called";
}

void SynchEntity_S::Register( CHlaRuntime_S *runtime )
{
	// verification
	if(m_instanceID!=HLAInstanceID_None)
		throw "Invalid HLA instance ID state!";

	if(m_runtime!=runtime)
	{
		Unregister();

		m_instanceID=runtime->CreateInstanceID();

		// verification
		if(runtime->m_synchEntities.find(m_instanceID)!=runtime->m_synchEntities.end())
		{
			throw "duplicated SynchEntity ID during registration!";
		}

		// commit 
		runtime->m_synchEntities.insert(SynchEntities_S::value_type(m_instanceID,this));
		m_runtime=runtime;

		m_runtime->Check_OneSynchEntity_EveryViewport(this);
	}
}

void SynchEntity_S::Unregister()
{
	if(m_runtime!=NULL)
	{
		m_runtime->RemoveOneSynchEntity_INTERNAL(this);
		m_runtime->m_synchEntities.erase(m_instanceID);
		m_runtime=NULL;
	}
}

void SynchEntity_S::CreateAppearMessageOnNeed_INTERNAL( CMessage& output )
{
	if(output.GetLength()==0)
	{
		ASSERT(GetInstanceID()!=HLAInstanceID_None);
		Message_Write(output,HLAMessageType_Appear);
		Message_Write(output,GetInstanceID());
		Message_Write(output,GetClassID());
		GatherAllToMessage(output);
	}
}

void SynchEntity_S::CreateDisappearMessageOnNeed_INTERNAL( CMessage& output )
{
	if(output.GetLength()==0)
	{
		ASSERT(GetInstanceID()!=HLAInstanceID_None);
		Message_Write(output,HLAMessageType_Disappear);
		Message_Write(output,GetInstanceID());		
	}
}

void SynchEntity_S::GetViewportOwnerHostList( vector<HLAHostID> &outHostList )
{
	outHostList.clear();
	for(SynchViewportList::iterator i=m_beholders_INTERNAL.begin();i!=m_beholders_INTERNAL.end();i++)
	{
		SynchViewport_S* viewport=*i;
		outHostList.push_back(viewport->GetHostID());
	}
}

SynchEntity_C::~SynchEntity_C()
{

}