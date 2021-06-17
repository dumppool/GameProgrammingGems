//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef ACTION_H
#define ACTION_H

#ifdef _MSC_VER
#pragma once
#endif

#include <string>
#include <list>

#include "../AStar/Node.h"
#include "../WorldProperty.h"
#include "ContextPrecondition.h"

namespace AI {
	class Action;
	typedef std::list<Action *> ActionContainer;

	//The result of activating an action.
	enum ActionResult {
		ACTION_IN_PROGRESS, //Action has not yet completed
		ACTION_COMPLETE,	//Action is finished executing.
		ACTION_FAILED		//Action failed to execute. Note that this means the actor couldn't do the action,
							// not that the action itself failed. For example, firing a missile in AttackWithMissiles
							// is ACTION_COMPLETE, even if the missile misses. AttackWithMissiles would return ACTION_FAILED
							// if the agent had no missiles left.
	};


	class Action : public Node
	{
	
	public:
		Action();
		Action(const Action& rhs);
		virtual ~Action();
		virtual Action* Clone() const;

		virtual ActionResult Activate(Agent &agent, float &timeInAction);

		static const std::string m_name;

		const ActionContainer& GetActionsToPrecede() const;
		
		virtual const PropertyList& GetPreconditions() const;
		virtual const PropertyList& GetEffects() const;
		virtual const ContextPreconditionContainer& GetContextPreconditions() const;

		virtual const std::string& GetName() const;
		virtual void SetLocation(WorldPoint location);
		virtual WorldPoint GetLocation() const;
		virtual bool operator==(const Action &rhs) const;
	protected:
		PropertyList m_preconditions;
		PropertyList m_effects;
		ContextPreconditionContainer m_contextPreconditions;

		ActionContainer m_actionsToPrecede;
	};

}

#endif //ACTION_H