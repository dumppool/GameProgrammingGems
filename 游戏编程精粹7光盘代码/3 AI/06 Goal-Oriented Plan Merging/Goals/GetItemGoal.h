//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_GOALS_GETITEMGOAL
#define AI_GOALS_GETITEMGOAL

#include "Goal.h"

namespace AI
{
	class GetItemGoal : public Goal
	{
	public:
		GetItemGoal();
		GetItemGoal(WorldPoint location);
		~GetItemGoal();

		virtual const std::string& GetName() const;

		virtual WorldPoint GetLocation() const;
		void SetLocation(const WorldPoint location);
	protected:
		WorldPoint m_location;
	};

}

#endif //AI_GOALS_GETITEMGOAL