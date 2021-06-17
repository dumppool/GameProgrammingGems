//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_ASTAR_STORAGE_H
#define AI_ASTAR_STORAGE_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Node.h"

namespace AI {
	class Storage 
	{
	public:
		Storage();
		virtual ~Storage() = 0;

		virtual bool IsOpenListEmpty() const = 0;
		virtual void PushOntoClosed(Node *node);
		virtual void PushOntoOpen(Node *node);
		virtual void UpdateNodeOnOpen(Node *node);

		virtual Node* PopBestNodeFromOpen();
		virtual Node* GetLastNodeOnClosed();

	};
}

#endif