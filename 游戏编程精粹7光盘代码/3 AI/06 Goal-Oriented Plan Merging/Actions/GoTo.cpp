//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "GoTo.h"

#include "Attack.h"
#include <sstream>

#include "../World.h"
extern World g_world;

using namespace AI;

const std::string GoTo::m_name = "GoTo";

GoTo::GoTo() : m_location(-1,-1)
{
	WorldProperty goToEffect(kAtLocation, eBool);
	goToEffect.m_value.m_bValue = true;

	m_effects.push_back(goToEffect);
}
GoTo::GoTo(WorldPoint location) {
	WorldProperty goToEffect(kAtLocation, eBool);
	goToEffect.m_value.m_bValue = true;
	
	m_location = location;

	m_effects.push_back(goToEffect);
}
GoTo::GoTo(const GoTo& rhs) : Action(rhs) {
	m_location = rhs.m_location;
}
GoTo::~GoTo() {}
GoTo* GoTo::Clone() const {
	return new GoTo(*this);
}

//GoTo* GoTo::Instance()
//{
//	static GoTo goTo;
//	return &goTo;
//}

void GoTo::SetLocation(WorldPoint location) {
	m_location = location;
}
WorldPoint GoTo::GetLocation() const {
	return m_location;
}

const std::string& GoTo::GetName() const
{
	std::stringstream buffer;
	buffer << m_name
			<< "("
			<< m_location.first
			<< ", "
			<< m_location.second
			<< ")";
	m_nameAndLocation = buffer.str();
	return m_nameAndLocation;
}

ActionResult GoTo::Activate(Agent &agent, float &timeInAction)
{
	if(m_location.first == -1 &&
		m_location.second == -1) 
	{
		//we've never been here before. Get our goal's location
		m_location = agent.GetLastGoal()->GetLocation();
	}

	if(agent.GetXPos() != m_location.first) {
		if(m_location.first > agent.GetXPos()) {
			agent.SetXPos(agent.GetXPos() + 1);
		} else {
			agent.SetXPos(agent.GetXPos() - 1);
		}
		return ACTION_IN_PROGRESS;
	} else if(agent.GetYPos() != m_location.second) {
		if(m_location.second > agent.GetYPos()) {
			agent.SetYPos(agent.GetYPos() + 1);
		} else {
			agent.SetYPos(agent.GetYPos() - 1);
		}
		return ACTION_IN_PROGRESS;
	} else {
		m_location.first = m_location.second = -1;
		return ACTION_COMPLETE;
	}
}
