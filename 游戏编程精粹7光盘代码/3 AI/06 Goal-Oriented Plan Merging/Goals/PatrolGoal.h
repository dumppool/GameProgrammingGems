//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_GOALS_PATROLGOAL
#define AI_GOALS_PATROLGOAL

#include "Goal.h"

namespace AI
{
	class PatrolGoal : public Goal
	{
	public:
		PatrolGoal();
		~PatrolGoal();

		virtual const std::string& GetName() const;
	protected:
	};

}

#endif //AI_GOALS_PATROLGOAL