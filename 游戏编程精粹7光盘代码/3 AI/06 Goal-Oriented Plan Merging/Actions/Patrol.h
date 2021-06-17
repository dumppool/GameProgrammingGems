//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_ACTIONS_PATROL
#define AI_ACTIONS_PATROL

#ifdef _MSC_VER
#pragma once
#endif

#include "Action.h"
#include "../Agent.h"

namespace AI 
{
	class Patrol : public Action
	{
	public:
		Patrol();
		virtual ~Patrol();
		Patrol* Clone() const;
		
		static const std::string m_name;
		virtual ActionResult Activate(Agent &agent, float &timeInAction);
	protected:

		WorldPoint m_currentPatrolPoint;
	public:
		static Patrol* Instance();		
		virtual const std::string& GetName() const;
	};
}

#endif //AI_ACTIONS_PATROL