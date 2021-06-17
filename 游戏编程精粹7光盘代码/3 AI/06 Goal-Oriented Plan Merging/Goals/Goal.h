//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_ASTAR_GOAL_H
#define AI_ASTAR_GOAL_H

#ifdef _MSC_VER
#pragma once
#endif

#include "../Actions/Action.h"
#include "../WorldProperty.h"


namespace AI {
	class Plan;
	class Goal;

	typedef std::vector<std::string> GoalNameContainer;
	typedef std::vector<Goal *>		 GoalContainer;

	class Goal 
	{

	public:
		Goal(float priority);
		virtual ~Goal();

		void SetPriority(float priority);
		const float GetPriority() const;

		virtual bool IsComplete(const Plan *plan) const;
		virtual bool IsChildValid(const Action *child);
		virtual float DistanceFromChild(const Action *child);
		virtual WorldPoint GetLocation() const;

		void AddEffect(const WorldProperty& effect);
		void RemoveEffects(const Action *action);
		void AddPreconditions(const Action *action);

		const PropertyList& GetTargetStates() const;
		Goal GetGoalAfterPlan(const Plan *plan) const;
	
		static Goal MergeGoals(const Goal *firstGoal, const Goal *secondGoal, bool& merged);

		virtual const std::string& GetName() const;

		Goal* operator=(const Goal& rhs);
		bool operator==(const Goal& rhs);
	protected:
		PropertyList				m_targetStates;
		GoalNameContainer			m_exclusiveGoals;
		std::string					m_name;
		float						m_priority;
	};
}

#endif