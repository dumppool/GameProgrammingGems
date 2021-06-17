//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Plan.h"
#include <algorithm>
#include <iostream>

using namespace AI;

Plan::Plan()  {}
Plan::Plan(const Action *action)
{
	while(action != NULL) 
	{
		m_actions.push_front(const_cast<Action *>(action));	
		action = reinterpret_cast<const Action *>(action->GetParent());
	}

	m_actionIter = m_actions.begin();
}

Plan::Plan(const Plan &plan)
{
	m_actions	= plan.m_actions;
	m_actionIter = m_actions.begin();
}

Plan& Plan::operator=(const Plan &rhs)
{
	if(this == &rhs)
	{
		return *this;
	}
	m_actions	= rhs.m_actions;
	m_actionIter = m_actions.begin();

	return *this;
}

void Plan::CreatePlan(const Action *action)
{
	m_actions.clear();

	//TODO: make sure actions precede actions they're supposed to
	while(action != NULL) 
	{
		if(! action->GetActionsToPrecede().empty())
		{
			//If there are actions this one is supposed to precede, check to see if it's already in the plan
			bool didInsertAction = false;

			ActionContainer::const_iterator i = action->GetActionsToPrecede().begin();
			for( ; i != action->GetActionsToPrecede().end(); ++i)
			{
				ActionContainer::iterator actionToCheck = std::find(m_actions.begin(), m_actions.end(), *i);
				if(actionToCheck != m_actions.end())
				{
					//we found an action in the list that this one was supposed to be in front of
					++actionToCheck;
					m_actions.insert(actionToCheck, const_cast<Action *>(action)); //put our new action right after the action we found
						//remember that these plans are in reverse order
					didInsertAction = true;
					break;
				} else {
					m_actions.push_front(const_cast<Action *>(action));
					didInsertAction = true;
					break;
				}
			}
			
			if(!didInsertAction)
			{
				m_actions.push_front(const_cast<Action *>(action));
			}


		} else {
			//Otherwise, just stick this action at the front of the list.		
//			m_actions.insert(m_actions.begin(), const_cast<Action*>(action));
			m_actions.push_front(const_cast<Action *>(action));
		}
		action = reinterpret_cast<const Action *>(action->GetParent());
	}

	m_actions.reverse();
	m_actionIter = m_actions.begin();
}

Plan::~Plan() {
	m_actions.resize(0);
	m_actions.swap(m_actions);
}

const ActionContainer& Plan::GetActions() const
{
	return m_actions;
}

std::string Plan::GetPlanString() {
	if(m_actions.empty()) 
	{
		return std::string("Empty plan");
	}
	std::string retval = "";
	ActionContainer::const_iterator i = m_actions.begin();
	retval += (*i)->GetName();
	++i;

	for( ; i != m_actions.end(); ++i)
	{
		retval += " -> \n ";
		retval += (*i)->GetName();
	}
	return retval;
}

void Plan::Print() const
{
	//if(m_actions.empty()) 
	//{
	//	std::cout << "Empty plan" << std::endl;
	//	return;
	//}
	//ActionContainer::const_iterator i = m_actions.begin();
	//std::cout << (*i)->GetName();
	//++i;

	//for( ; i != m_actions.end(); ++i)
	//{
	//	std::cout << " -> " << (*i)->GetName();
	//}
	//std::cout << std::endl;
}

void Plan::PrintReverse() const
{
	//if(m_actions.empty()) 
	//{
	//	std::cout << "Empty plan" << std::endl;
	//	return;
	//}
	//ActionContainer::const_reverse_iterator i = m_actions.rbegin();
	//std::cout << (*i)->GetName();
	//++i;

	//for( ; i != m_actions.rend(); ++i)
	//{
	//	std::cout << " -> " << (*i)->GetName();
	//}
	//std::cout << std::endl;
}

Plan Plan::MergePlans(const AI::Plan &firstPlan, const AI::Plan &secondPlan, bool& merged)
{
	merged = false;

	//Here, we take the strategy to look for actions we know we can merge
	ActionContainer::const_iterator firstPlanIter, secondPlanIter;
	for(firstPlanIter = firstPlan.m_actions.begin(); firstPlanIter != firstPlan.m_actions.end(); ++firstPlanIter) {
		if((*firstPlanIter)->GetName() == "ReturnItemAction") {
			break;
		}
	}
	if(firstPlanIter == firstPlan.m_actions.end()) {
		return firstPlan;
	}

	for(secondPlanIter = secondPlan.m_actions.begin(); secondPlanIter != secondPlan.m_actions.end(); ++secondPlanIter) {
		if((*secondPlanIter)->GetName() == "ReturnItemAction") {
			break;
		}
	}
	if(secondPlanIter == secondPlan.m_actions.end()) {
		return firstPlan;
	}

	//If we make it here, we can merge the plans on ReturnItemAction
	Plan mergedPlan;
	mergedPlan.m_actions.insert(mergedPlan.m_actions.end(), firstPlan.m_actions.begin(), firstPlanIter);
	mergedPlan.m_actions.insert(mergedPlan.m_actions.end(), secondPlan.m_actions.begin(), secondPlanIter);
	mergedPlan.m_actions.push_back(*firstPlanIter);
	mergedPlan.m_actions.insert(mergedPlan.m_actions.end(), ++firstPlanIter, firstPlan.m_actions.end());
	mergedPlan.m_actions.insert(mergedPlan.m_actions.end(), ++secondPlanIter, secondPlan.m_actions.end());
	
	merged = true;
	return mergedPlan;
}

void Plan::Clear()
{
	m_actions.clear();
}

bool Plan::IsEmpty()
{
	return m_actions.empty();
}

Action* Plan::GetNextAction()
{
	if(m_actionIter != m_actions.end())
	{
		Action *action = *m_actionIter;
		++m_actionIter;
		return action;
	}
	else 
	{
		m_actions.clear();
		return NULL;
	}
}
