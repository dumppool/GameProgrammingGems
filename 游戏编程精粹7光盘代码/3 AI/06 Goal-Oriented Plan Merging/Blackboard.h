//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef BLACKBOARD
#define BLACKBOARD

#ifdef _MSC_VER
#pragma once
#endif

#include <set>
#include <hash_map>

enum BlackboardType {
	BB_ATTACKING_PLAYER,
	BB_SQUAD_HAS_LEADER,
};

class Blackboard {
	typedef int BlackboardContainerValue;
	typedef std::set<BlackboardContainerValue> BlackboardFact;
	typedef BlackboardFact::iterator BlackboardFactIterator;
	typedef stdext::hash_map<BlackboardType, BlackboardFact> BlackboardContainer;
	typedef BlackboardContainer::iterator BlackboardContainerIterator;

public:
	static Blackboard* Instance();

	int LookupFact(BlackboardType type, int arg = 0);
	void PostFact(BlackboardType type, BlackboardContainerValue value);
	void RemoveFact(BlackboardType type, BlackboardContainerValue value);

private:
	Blackboard();

	BlackboardContainer m_container;

};

#endif //BLACKBOARD