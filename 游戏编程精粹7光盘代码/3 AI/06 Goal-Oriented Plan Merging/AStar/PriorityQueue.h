//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>
#include <algorithm>
#include "Node.h"

namespace AI {

	class NodeTotalGreater {
	public:
		bool operator()(Node *first, Node *second) const {
			return (first->GetTotal() > second->GetTotal());
		}
	};

	class PriorityQueue {
	public:
		std::vector<Node *> heap;

		bool IsEmpty() const;
		
		Node* Pop();
		void Push(Node* node);
		
		void UpdateNode(Node* node);
		void Clear();
	};
}
#endif