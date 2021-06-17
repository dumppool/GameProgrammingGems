//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "PatrolGoal.h"

using namespace AI;

PatrolGoal::PatrolGoal() : Goal(0.1f)
{
	m_name = "PatrolGoal";
	WorldProperty prop(kPatrolling, eBool);
	prop.m_value.m_bValue = true;

	m_targetStates.push_back(prop);
}

PatrolGoal::~PatrolGoal() {}

const std::string& PatrolGoal::GetName() const
{
	return m_name;
}