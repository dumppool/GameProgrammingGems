//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef PLAN_H
#define PLAN_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Goals/Goal.h"
#include "Actions/Action.h"

namespace AI {

	class GoalPriorityGreater {
	public:
		bool operator()(Goal *first, Goal *second) const {
			return (first->GetPriority() > second->GetPriority());
		}
	};
	
	class Plan
	{
	public:
		Plan();
		Plan(const Action *action);
		Plan(const Plan& plan);
		Plan& operator=(const Plan& rhs);
		~Plan();

		const ActionContainer& GetActions() const;

		std::string GetPlanString();
		void Print() const;
		void PrintReverse() const;
		void CreatePlan(const Action *action);

		static Plan MergePlans(const Plan& firstPlan, const Plan& secondPlan, bool &merged);

		void Clear();
		bool IsEmpty();

		Action* GetNextAction();

	private:
		ActionContainer m_actions;
		ActionContainer::iterator m_actionIter;
	};
}
#endif //PLAN_H