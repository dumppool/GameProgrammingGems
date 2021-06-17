//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "PriorityQueueStorage.h"

using namespace AI;

PriorityQueueStorage::PriorityQueueStorage()  {}
PriorityQueueStorage::~PriorityQueueStorage() {
	std::vector<Node *>::iterator iter = m_closedList.begin();
	for( ; iter != m_closedList.end(); ++iter) {
		delete *iter;
	}
}

bool PriorityQueueStorage::IsOpenListEmpty() const 
{
	return m_openList.IsEmpty();
}

void PriorityQueueStorage::PushOntoClosed(Node *node)
{
	m_closedList.push_back(node);
}

void PriorityQueueStorage::PushOntoOpen(Node *node)
{
	m_openList.Push(node);
}

void PriorityQueueStorage::UpdateNodeOnOpen(Node *node)
{
	m_openList.UpdateNode(node);
}

Node* PriorityQueueStorage::PopBestNodeFromOpen()
{
	return m_openList.Pop();
}

Node* PriorityQueueStorage::GetLastNodeOnClosed()
{
	return m_closedList.back();
}