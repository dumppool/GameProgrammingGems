//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "ActionDictionary.h"

using namespace AI;

#include <utility>

ActionDictionary::ActionDictionary(ActionContainer& actions)  
{
	for(ActionContainer::iterator actionIter = actions.begin(); actionIter != actions.end(); ++actionIter)
	{
		PropertyList effects;
		effects = (*actionIter)->GetEffects();
	
		for(PropertyList::iterator i = effects.begin(); i != effects.end(); ++i)
		{
			m_actions.insert( ActionMap::value_type( (*i).m_key, *actionIter ) );
		}
	}
}

ActionDictionary::~ActionDictionary() 
{
	ActionMap::iterator iter = m_actions.begin();
	for( ; iter != m_actions.end(); ++iter) {
		delete iter->second;
	}
	m_actions.clear();
}

ActionMap* ActionDictionary::GetAllActions() {
	return &m_actions;
}

//ActionDictionary* ActionDictionary::Instance() 
//{
//	static ActionDictionary dictionary;
//	return &dictionary;
//}

ActionContainer& ActionDictionary::GetActionsForProperty(const WorldProperty &prop) const
{
	std::pair<ActionMap::const_iterator, ActionMap::const_iterator> matchingRange = m_actions.equal_range(prop.m_key); 

	m_matchingActions.clear();
	//m_matchingActions.reserve(m_actions.count(prop.m_key));

	for(ActionMap::const_iterator i = matchingRange.first; i != matchingRange.second; ++i)
	{
		m_matchingActions.push_back( i->second );
	}

	return m_matchingActions;
}

void ActionDictionary::ResetActionsForSearch()
{
	ActionMap::iterator i = m_actions.begin();
	for( ; i != m_actions.end(); ++i)
	{
		(*i).second->Reset();
	}
}