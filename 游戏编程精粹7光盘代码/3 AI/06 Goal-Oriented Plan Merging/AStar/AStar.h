//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef ASTAR_H
#define ASTAR_H

#ifdef _MSC_VER
#pragma once
#endif

#include "../Goals/Goal.h"
#include "Storage.h"
#include "Node.h"
#include "../Actions/Action.h"
#include "../Actions/ActionDictionary.h"
#include "../Agent.h"

namespace AI {
	class AStar {

	public:
		AStar();
		~AStar();

		//Setup a new search using the folowing Goal, Storage class, and startNode. 
		// It is assumed that the goal and storage classes are not released during the search. 
		// Maybe I should change this later? --TODO, Michael, 10/6/2006
		void SetupSearch(Goal *goal, Agent *agent, Storage *storage);
		void Search();

		bool IsPathFound() const;
		bool IsSearchFinished() const;

		Node* GetResult() const;

	private:
		float m_heuristicWeight;

		Storage *m_storage;
		Goal	*m_goal;
		Agent	*m_agent;
		bool	m_searchFinished;
		bool	m_pathFound;
	};
} //AI namespace

#endif