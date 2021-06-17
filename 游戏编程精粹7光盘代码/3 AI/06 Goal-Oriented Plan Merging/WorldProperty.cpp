//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "WorldProperty.h"
#include <math.h>

using namespace AI;

WorldProperty::WorldProperty(WorldPropertyKey key, WorldPropertyValueType type)  
{
	m_key  = key;
	m_type = type;
}

WorldProperty::~WorldProperty() {}

bool WorldProperty::operator==(const WorldProperty &rhs) const
{
	if(m_key != rhs.m_key) {
		return false;
	}
	if(m_type != rhs.m_type) {
		return false;
	}
	switch(m_type)
	{
	case(eBool):
		return m_value.m_bValue == rhs.m_value.m_bValue;
		break;
	case(eFloat):
		return fabs(m_value.m_fValue - rhs.m_value.m_fValue) < 0.001f;
		break;
	case(eInt):
		return m_value.m_nValue == rhs.m_value.m_nValue;
		break;
	}
	return false;
}