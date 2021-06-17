#include "AgentFactory.h"

//Goals
#include "Goals/PatrolGoal.h"
#include "Goals/GetItemGoal.h"


//Actions
#include "Actions/Patrol.h"
#include "Actions/GetItemAction.h"
#include "Actions/GoTo.h"
#include "Actions/ReturnItemAction.h"

#include "debug.h"

#include "World.h"
extern World g_world;

#include <algorithm>

using namespace AI;

AgentFactory::AgentFactory() {}

AgentFactory* AgentFactory::Instance() {
	static AgentFactory factory;
	return &factory;
}

Agent* AgentFactory::CreateAgent(AI::AgentType type, AI::Team team) {
	ActionContainer actions;
	GoalContainer goals;

	if(type == AGENT_NONE) {
		//blank agent
	} else if(type == AGENT_PATROL) {
		//These goal pointers are destroyed with the Agent
		goals.push_back(new PatrolGoal());

		//These pointers are copied into an Agent's ActionDictionary (m_allValidActions) and are cleaned up
		// when the ActionDictionary is destructed.
		actions.push_back(new Patrol());
		actions.push_back(new GetItemAction());
		actions.push_back(new GoTo());
		actions.push_back(new ReturnItemAction(g_world.GetBaseLocation(team)));
	}
	else {
		dprintf("Warning! Undefined agent type created!");
	}
	std::make_heap(goals.begin(), goals.end(), GoalPriorityGreater());
	return new Agent(actions, goals, type, team);
}