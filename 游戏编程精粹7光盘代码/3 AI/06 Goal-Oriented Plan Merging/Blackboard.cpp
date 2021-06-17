//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Blackboard.h"

#include <algorithm>

Blackboard::Blackboard() {}

Blackboard* Blackboard::Instance() {
	static Blackboard board;
	return &board;
}

int Blackboard::LookupFact(BlackboardType type, int arg /*= 0*/) {
	BlackboardFactIterator iter;
	iter = m_container[type].find(arg);
	if(iter == m_container[type].end()) {
		return -1;
	}
	return *iter;
}

void Blackboard::PostFact(BlackboardType type, BlackboardContainerValue value) {
	m_container[type].insert(value);
}

void Blackboard::RemoveFact(BlackboardType type, BlackboardContainerValue value) {
	m_container[type].erase(value);
}