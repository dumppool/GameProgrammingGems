//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef ACTIONDICTIONARY_H
#define ACTIONDICTIONARY_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Action.h"
#include "Attack.h"
#include "../WorldProperty.h"

#include <hash_map>
#include <vector>

namespace AI {
	typedef stdext::hash_multimap<WorldPropertyKey, Action*> ActionMap; 

	class ActionDictionary
	{
	public:
		//static ActionDictionary* Instance();
		ActionDictionary(ActionContainer& actions);
		~ActionDictionary();
		
		ActionContainer& GetActionsForProperty(const WorldProperty &prop) const;

		void ResetActionsForSearch();

		ActionMap* GetAllActions();

	private:

		ActionMap		m_actions;
		mutable ActionContainer m_matchingActions;

	};

}
#endif //ACTIONDICTIONARY_H