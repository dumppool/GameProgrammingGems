//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "GetItemGoal.h"

using namespace AI;

GetItemGoal::GetItemGoal() : Goal(0.0f), m_location(-1,-1)
{
	m_name = "GetItemGoal";
	WorldProperty prop(kReturnedItem, eBool);
	prop.m_value.m_bValue = true;

	m_targetStates.push_back(prop);
}
GetItemGoal::GetItemGoal(WorldPoint location) : Goal(0.0f), m_location(location) {
	m_name = "GetItemGoal";
	WorldProperty prop(kReturnedItem, eBool);
	prop.m_value.m_bValue = true;

	m_targetStates.push_back(prop);
}

GetItemGoal::~GetItemGoal() {}

const std::string& GetItemGoal::GetName() const
{
	return m_name;
}

WorldPoint GetItemGoal::GetLocation() const {
	return m_location;
}
void GetItemGoal::SetLocation(const WorldPoint location) {
	m_location = location;
}
