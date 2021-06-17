//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_AGENT_H
#define AI_AGENT_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Actions/ActionDictionary.h"
#include "Goals/Goal.h"
#include "Plan.h"

#include "WorkingMemoryFact.h"

#include "LuaWrapper.h"

#include <utility>

namespace AI {
	enum AgentType;
	enum Team;

	typedef std::pair<int, int> WorldPoint;

	class FactMemory {
		typedef std::vector<WorkingMemoryFact>	FactContainer;
		typedef FactContainer::iterator			FactIterator;
	public:
		FactContainer m_facts;

		WorkingMemoryFact& GetFactById(const long handle);
		void DecayMemory();
	};

	class Agent
	{
		typedef std::vector<WorldPoint> WorldPointContainer;

	public:
		Agent(ActionContainer& actions, GoalContainer& goals, AgentType type, Team team);
		~Agent();

		void Load(Lua::LuaWrapper *lua, std::string agentTable);

		FactMemory m_memory;
		void DecayMemory();

		const ActionContainer& GetValidActionsForProperty(const WorldProperty &prop);

		ActionDictionary*	GetActionDictionary();
		GoalContainer*		GetAllGoals();
		Plan*				GetCurrentPlan();
		Plan*				GetSecondaryPlan();

		bool MakePlan(Goal *goal, Plan *plan);
		bool HasCurrentPlan();
		bool ShouldReplan();
		void SetShouldReplan(bool replan);
		void ClearCurrentAction();

		Goal* GetBestGoal();
		Goal* GetSecondGoal();
		Goal* GetLastGoal();
		void SetLastGoal(Goal *goal);
		void FailGoal(Goal *goal);

		void ResetActionsForSearch();
		void ExecutePlan();

		void SetHasMissiles(const bool missiles);
		bool HasMissiles() const;
		void SetHasBullets(const bool bullets);
		bool HasBullets() const;

		void SetHasAttacked(const bool attacked);
		bool HasAttacked();

		void		AddPatrolPoint(WorldPoint point);
		void		ResetPatrolPointIterator();
		void		NextPatrolPoint();
		WorldPoint  GetCurrentPatrolPoint();

		void		SetCurrentTargetLocation(WorldPoint location);
		WorldPoint  GetCurrentTargetLocation() const;

		void AddItem(WorldPoint location);
		WorldPoint DropItem();
		bool HasItems() const;

		Team GetTeam() const;

		bool UpdateSensors(const long id);

		void Update();

		void Die();

		void SetXPos(unsigned int pos);
		void SetYPos(unsigned int pos);
		int GetXPos() const;
		int GetYPos() const;

	private:
		unsigned int m_xPos, m_yPos;

		WorldPoint m_currentTargetLocation;

		ActionDictionary m_allValidActions;
		ActionContainer  m_currentValidActions;

		GoalContainer	m_allGoals;
		GoalContainer	m_failedGoals;

		Goal *m_lastGoal;
		Plan m_plan;
		Plan m_secondPlan;

		bool m_hasMissiles;
		bool m_hasBullets;
		bool m_coverAvailable;

		bool m_shouldReplan;

		bool m_hasAttacked;

		Action *m_currentAction;
		ActionResult m_currentActionResult;

		WorldPointContainer m_patrolPoints;
		WorldPointContainer ::const_iterator m_patrolPointIterator;

		WorldPointContainer m_items;

		float m_timeInAction;

		unsigned short m_squadNumber;

		AgentType m_type;
		Team m_team;
	};

}

#endif