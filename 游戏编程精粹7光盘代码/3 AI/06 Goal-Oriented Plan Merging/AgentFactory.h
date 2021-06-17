#ifndef AGENTFACTORY_H
#define AGENTFACTORY_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Agent.h"

namespace AI {
	enum AgentType {
		AGENT_NONE = 0,
		AGENT_PATROL,
	};
	enum Team {
		TEAM_BLUE = 0,
		TEAM_RED,
	};

	class AgentFactory {
	public:
		static AgentFactory* Instance();
		Agent* CreateAgent(AgentType type, Team team);
	private:
		AgentFactory();
	};
}	
#endif //AGENTFACTORY_H