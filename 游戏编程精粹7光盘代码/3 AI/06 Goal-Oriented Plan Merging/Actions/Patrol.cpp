//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe


#include "Patrol.h"

#include "../global.h"
#include "../debug.h"
#include "../Agent.h"

using namespace AI;

const std::string Patrol::m_name = "Patrol";

Patrol::Patrol() : m_currentPatrolPoint(0,0)
{
	WorldProperty patrolEffect(kPatrolling, eBool);
	patrolEffect.m_value.m_bValue = true;

	m_effects.push_back(patrolEffect);
}

Patrol::~Patrol() {}
Patrol* Patrol::Clone() const {
	return new Patrol(*this);
}


Patrol* Patrol::Instance() {
	static Patrol patrol;
	return &patrol;
}

const std::string& Patrol::GetName() const
{
	return m_name;
}

ActionResult Patrol::Activate(Agent &agent, float &timeInAction)
{
	if(agent.GetXPos() != agent.GetCurrentPatrolPoint().first) {
		if(agent.GetCurrentPatrolPoint().first > agent.GetXPos()) {
			agent.SetXPos(agent.GetXPos() + 1);
		} else {
			agent.SetXPos(agent.GetXPos() - 1);
		}
		return ACTION_IN_PROGRESS;
	} else if(agent.GetYPos() != agent.GetCurrentPatrolPoint().second) {
		if(agent.GetCurrentPatrolPoint().second > agent.GetYPos()) {
			agent.SetYPos(agent.GetYPos() + 1);
		} else {
			agent.SetYPos(agent.GetYPos() - 1);
		}
		return ACTION_IN_PROGRESS;
	} else {
		agent.NextPatrolPoint();
		return ACTION_COMPLETE;
	}
}