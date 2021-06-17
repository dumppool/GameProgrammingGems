//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_ASTAR_PRIORITY_QUEUE_STORAGE_H
#define AI_ASTAR_PRIORITY_QUEUE_STORAGE_H

#ifdef _MSC_VER
#pragma once
#endif

#include "PriorityQueue.h"
#include "Storage.h"

namespace AI {
	class PriorityQueueStorage : public Storage
	{
	public:
		PriorityQueueStorage();
		virtual ~PriorityQueueStorage();

		virtual bool IsOpenListEmpty() const;
		virtual void PushOntoClosed(Node *node);
		virtual void PushOntoOpen(Node *node);
		virtual void UpdateNodeOnOpen(Node *node);

		virtual Node* PopBestNodeFromOpen();
		virtual Node* GetLastNodeOnClosed();
	private:
		PriorityQueue		m_openList;
		std::vector<Node*>	m_closedList;
	};
}

#endif