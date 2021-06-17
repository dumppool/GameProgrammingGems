//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "PriorityQueue.h"
#include <assert.h>

using namespace AI;

bool PriorityQueue::IsEmpty() const {
	return heap.empty();
}

void PriorityQueue::Clear() {
	heap.clear();
}

//As given in lecture notes. O(log n) time.
Node* PriorityQueue::Pop() {
	//First node has lowest cost
	Node *node = heap.front();

	//Moves first node to back. Sorts everything else.
	std::pop_heap(heap.begin(), heap.end(), NodeTotalGreater());

	//Take that last node off
	heap.pop_back();

	return node;
}

//As given in lecture notes. O(log n) time
void PriorityQueue::Push(Node* node) {
	heap.push_back(node);

	//sort the heap
	std::push_heap(heap.begin(), heap.end(), NodeTotalGreater());

	//push_heap crashes if it's not a valid heap to begin with.
	//std::make_heap(heap.begin(), heap.end(), NodeTotalGreater());
}

//As given in lecture notes. O(n + log n) time
void PriorityQueue::UpdateNode(Node* node) {
	std::vector<Node *>::iterator i;
	for(i = heap.begin(); i != heap.end(); ++i) {
		if( (*i) == node )
		{
			//resort the heap after this position
			std::push_heap(heap.begin(), i + 1, NodeTotalGreater());

			//we're done.
			return;
		}
	}

	assert("Tried to update a nonexistent node");
	return;
}