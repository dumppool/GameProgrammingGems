//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Goal.h"
#include "../Plan.h"
#include <algorithm>

using namespace AI;


Goal::Goal(float priority) : m_name("Goal"), m_priority(priority)
 {}
Goal::~Goal() {}

bool Goal::IsComplete(const Plan *plan) const
{
	Goal lastState = GetGoalAfterPlan(plan);
	return lastState.m_targetStates.empty();
}

WorldPoint Goal::GetLocation() const {
	return WorldPoint(-1, -1);
}

const std::string& Goal::GetName() const
{
	return m_name;
}

const float Goal::GetPriority() const {
	return m_priority;
}
void Goal::SetPriority(float priority) {
	m_priority = priority;
}

Goal* Goal::operator=(const Goal& rhs) 
{
	if(this == &rhs) {
		return this;
	}

	this->m_exclusiveGoals.clear();
	this->m_targetStates.clear();

	std::copy(rhs.m_exclusiveGoals.begin(), rhs.m_exclusiveGoals.end(), this->m_exclusiveGoals.begin());
	std::copy(rhs.m_targetStates.begin(), rhs.m_targetStates.end(), this->m_targetStates.begin());

	return this;
}

bool Goal::operator==(const Goal& rhs) {
	return GetName() == GetName();
}


Goal Goal::GetGoalAfterPlan(const Plan *plan) const
{
	Goal lastState(*this);

	//Figure out what the state of the world is if we do this action and any preceeding actions
	// i.e. what have we accomplished if we do the entire plan?
	const ActionContainer actions = plan->GetActions();
	ActionContainer::const_iterator actionIter = actions.begin();
	for( ; actionIter != actions.end(); ++actionIter) 
	{
		//Remove the effects from the goal
		lastState.RemoveEffects(*actionIter);
		//Add the action's preconditions
		lastState.AddPreconditions(*actionIter);
	}

	return lastState;
}

bool Goal::IsChildValid(const Action *child)
{
	PropertyList effects = child->GetEffects();
	for(unsigned int i = 0; i < effects.size(); ++i)
	{
		for(unsigned int j = 0; j < m_targetStates.size(); ++j)
		{
			if(effects[i] == m_targetStates[j])
			{
				//This action satisfies at least one of our preconditions.
				// We should also check to make sure it doesn't negate any of them...
				return true;
			}
		}
	}
	return false;
}

float Goal::DistanceFromChild(const Action *child)
{
	return (float)child->GetPreconditions().size();
	//return 1.0f;
}

void Goal::RemoveEffects(const Action *action)
{
	PropertyList effects = action->GetEffects();
	PropertyList::iterator targetEffect;

	for(unsigned int effectCount = 0; effectCount < effects.size(); ++effectCount)
	{
		targetEffect = std::find(m_targetStates.begin(), m_targetStates.end(), effects[effectCount]);
		if(targetEffect != m_targetStates.end())
		{
			m_targetStates.erase(targetEffect);
		}
	}
}

void Goal::AddPreconditions(const Action *action)
{
	PropertyList preconditions = action->GetPreconditions();

	for(unsigned int i = 0; i < preconditions.size(); ++i)
	{
		m_targetStates.push_back(preconditions[i]);
	}
}

const PropertyList& Goal::GetTargetStates() const
{
	return m_targetStates;
}

void Goal::AddEffect(const WorldProperty& effect)
{
	m_targetStates.push_back(effect);
}

//Takes two goals and returns one goal with the targetstates of both, if the goals are compatible.
// If the goals have incompatible target states (defined as at least two target states with the same key, but different value)
//  then MergeGoals returns firstGoal, and merged will == false.
// If the goals were merged, merged == true
Goal Goal::MergeGoals(const Goal *firstGoal, const Goal *secondGoal, bool& merged)
{
	GoalNameContainer::const_iterator i;
	i =	std::find(firstGoal->m_exclusiveGoals.begin(), firstGoal->m_exclusiveGoals.end(), secondGoal->GetName());

	if( i != firstGoal->m_exclusiveGoals.end() )
	{
		//These goals are exclusive to each other	
		merged = false;
		return *firstGoal;
	}

	Goal mergedGoal(*secondGoal);

	//	This double loop could be sped up considerably by keeping a hashtable of target states and values as we go through each list
	// However, so far the goal lists have been so short ( < 3), it isn't currently worth the overhead, and we're not 
	// even coming close to going over our allotted frame time
	PropertyList::const_iterator first, second;
	for(first = firstGoal->m_targetStates.begin(); first != firstGoal->m_targetStates.end(); ++first)
	{
		bool skip = false;
		for(second = secondGoal->m_targetStates.begin(); second != secondGoal->m_targetStates.end(); ++second)
		{
			if(*first == *second)
			{
				//the goals have a matching target state, so skip processing this step
				skip = true;			
				break;
			}
			// check for incompatible ending states
			//If these goals have a target state that is of the same type but different value, they're incompatible
			else if(first->m_key == second->m_key) //we know *first != *second because of the first if statement
			{
				merged = false;
				return *firstGoal;
			} 
		}

		if(skip) {
			continue;
		}
		//At this point, we've got a target state (first) that's not in our second goal, so add it
		mergedGoal.AddEffect(*first);
	}

	merged = true;
	return mergedGoal;
}

