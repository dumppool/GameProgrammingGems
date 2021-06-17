//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Action.h"

#include <iostream>

using namespace AI;

const std::string Action::m_name = "Action";

Action::Action() : Node()
{}
Action::Action(const Action& rhs) : Node(rhs) {
	std::copy(rhs.m_preconditions.begin(), rhs.m_preconditions.end(), std::back_inserter(m_preconditions));
	std::copy(rhs.m_effects.begin(), rhs.m_effects.end(), std::back_inserter(m_effects));
	std::copy(rhs.m_actionsToPrecede.begin(), rhs.m_actionsToPrecede.end(), std::back_inserter(m_actionsToPrecede));

}
Action::~Action() 
{
	m_preconditions.clear();
	m_effects.clear();
	m_actionsToPrecede.clear();
}
Action* Action::Clone() const {
	return new Action(*this);
}

const ActionContainer& Action::GetActionsToPrecede() const
{
	return m_actionsToPrecede;
}

const PropertyList& Action::GetPreconditions() const
{
	return m_preconditions;
}

const PropertyList& Action::GetEffects() const
{
	return m_effects;
}

const std::string& Action::GetName() const 
{
	return m_name;
}
void Action::SetLocation(AI::WorldPoint location) { } //purposely empty as default
WorldPoint Action::GetLocation() const { return WorldPoint(-1,-1); }


const ContextPreconditionContainer& Action::GetContextPreconditions() const
{
	return m_contextPreconditions;
}

ActionResult Action::Activate(Agent &agent, float &timeInAction)
{
	return ACTION_FAILED;
}

//Not cheap to do action comparisons this way, but it's simple
bool Action::operator==(const Action &rhs) const {
	return m_name == rhs.GetName();
}
