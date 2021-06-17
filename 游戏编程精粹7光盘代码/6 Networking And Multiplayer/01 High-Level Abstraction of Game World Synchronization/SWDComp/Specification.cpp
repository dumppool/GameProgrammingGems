//
// Copyright 2007 Hyun-jik Bae.  All rights reserved.
//
// Permission is granted to use this code for any purpose, as long as this
// copyright message remains intact.

#include "StdAfx.h"
#include ".\specification.h"
#include <antlr/SemanticException.hpp>

CSpecification::CSpecification(void)
{
}

CSpecification::~CSpecification(void)
{
}

void CSpecification::SetConfigs(CConfigurations *configs)
{
	m_configurations=*configs;
	m_configurations.m_incID=0;
}

void CSpecification::AddSynchClass(CSynchClass* func)
{
	m_synchClasses.push_back(*func);
	char incIDStr[100];
	sprintf(incIDStr,"%d",m_configurations.m_incID);
	m_synchClasses.back().m_ID.Format("(%s+%d)",m_configurations.m_startID,m_configurations.m_incID);
	m_configurations.m_incID++;
}

CConfigurations::CConfigurations()
{
	m_incID=0;
	m_startID="1";
	m_namespace="Unknown";
}

void CConfigurations::AddConfig(CConfiguration *config)
{
	if(config->m_name=="StartID")
	{
		m_startID=config->m_value;
	}
	else if(config->m_name=="Namespace")
	{
		m_namespace=config->m_value;
	}
	else
	{
		throw SemanticException("Unidentified config variable");
	}
}

void CConfiguration::SetName(CString name)
{
	m_name=name;
}

void CConfiguration::SetValue(CString value)
{
	m_value=value;
}

void CSynchClass::SetName(CString name)
{
	m_name=name;
}
void CSynchClass::AddMember(RefCount<CMember> member)
{
	m_memberIDGenerator++;
	member->m_ID=m_memberIDGenerator;
	m_members.push_back(member);
}

CSynchClass::CSynchClass()
{
	m_memberIDGenerator=0;
}

void CMember::SetType(CString type)
{
	m_type=type;
}
void CMember::SetName(CString name)
{
	m_name=name;
}

void CMember::SetBehavior( RefCount<CBehavior> behavior )
{
	m_behavior=behavior;
}

CMember::CMember()
{
	m_ID=0;
}

CBehavior::CBehavior()
{
	m_type=BehaviorType_Static;
}

void CBehavior::SetType( BehaviorType type )
{
	m_type=type;
}

BehaviorType CBehavior::GetType()
{
	return m_type;
}

LPCTSTR CBehavior::GetTemplateClassName()
{
	switch(m_type)
	{
	case BehaviorType_Conditional: 
		return "CConditionalBehaviorMember";
	case BehaviorType_Static:
		return "CStaticBehaviorMember";
	}
	throw "Invalid behavior is detected!";
}