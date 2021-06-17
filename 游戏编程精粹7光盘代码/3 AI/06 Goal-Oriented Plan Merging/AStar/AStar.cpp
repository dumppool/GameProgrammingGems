//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "AStar.h"
#include "../Plan.h"

using namespace AI;

#include <vector>

AStar::AStar() : m_heuristicWeight(1.0f) {}

AStar::~AStar() {}

bool AStar::IsPathFound() const
{
	return m_pathFound;
}

bool AStar::IsSearchFinished() const
{
	return m_searchFinished;
}

Node* AStar::GetResult() const
{
	if(!m_searchFinished)
	{
		return NULL;
	}
	return m_storage->GetLastNodeOnClosed();
}

void AStar::SetupSearch(Goal *goal, Agent *agent, Storage *storage)
{
	m_storage	= storage;
	m_goal		= goal;
	m_agent		= agent;

	m_agent->ResetActionsForSearch();

	PropertyList goalProperties = m_goal->GetTargetStates();
	PropertyList::const_iterator i = goalProperties.begin();
	ActionContainer actions;
	ActionContainer::const_iterator actionIterator;
	for( ; i != goalProperties.end(); ++i)
	{
		actions = m_agent->GetValidActionsForProperty(*i);
		for(actionIterator = actions.begin(); actionIterator != actions.end(); ++actionIterator)
		{
			m_storage->PushOntoOpen(const_cast<Action *>(*actionIterator));
		}
	}

	//This line would be used for a non-goal-planning Astar search, should we need one
	//m_storage->PushOntoOpen(startNode);
	m_searchFinished = false;
	m_pathFound		 = false;
}

void AStar::Search() {
	//if the open list is empty, we're done - no path is possible
	if(m_storage->IsOpenListEmpty()) {
		m_searchFinished = true;
		return;
	}

	//Pop best node. This is stored in the Storage class, on the closed list (i.e. gets deleted with Storage)
	Action *best =(static_cast<Action *>(m_storage->PopBestNodeFromOpen()))->Clone();
	best->SetOnOpen(false);
	WorldPoint loc = best->GetLocation();
	if(loc.first == -1 && loc.second == -1) {
		best->SetLocation(m_goal->GetLocation());
	}

	Plan currentPlan(best);
	//check to see if we've reached the goal
	if(m_goal->IsComplete(&currentPlan))
	{
		//we're done. path is found.
		m_storage->PushOntoClosed(best);
		best->SetOnClosed(true);	
		m_searchFinished	= true;
		m_pathFound			= true;
		return;
	}

	Goal currentGoal = m_goal->GetGoalAfterPlan(&currentPlan);

	//we're not at the goal, so now we need to add each "neighboring" node
	//std::vector<Action *> children = best->GetChildActions();
	PropertyList goalProperties = currentGoal.GetTargetStates();
	PropertyList::const_iterator i = goalProperties.begin();
	ActionContainer actions;
	ActionContainer::iterator actionIterator;
	for( ; i != goalProperties.end(); ++i)
	{
		actions = m_agent->GetValidActionsForProperty(*i);
		for(actionIterator = actions.begin(); actionIterator != actions.end(); ++actionIterator)
		{
			Action *child = *actionIterator;

			//float cost = best->GetCost() + 
			//	D3DXVec3Length( &(g_terrain.GetCoordinates(best->GetRow(), best->GetColumn()) - 
			//					  g_terrain.GetCoordinates(row, column)));
			float cost = best->GetCost() + 1.0f;    //Add 1 to cost as default cost...might want to have a seperate function
								// for adding the cost
			
			float distance = m_goal->DistanceFromChild(child);

			float total = cost + (m_heuristicWeight * distance);


			//check to see if the child is on the open or closed list first
			if(child->IsOnClosed() || child->IsOnOpen()) {
				//we only want to replace it if the cost is lower
				if(total < child->GetTotal())
				{
					if(child->IsOnClosed()) {
						//Update the path used to get to this node on the closed list
						child->SetParent(best);
						child->SetCost(cost);
						child->SetTotal(total);
					} else { //child was already on the open list
						child->SetParent(best);
						child->SetCost(cost);
						child->SetTotal(total);
						m_storage->UpdateNodeOnOpen(child);
		
						//these are already set, but this is here to remind you it's already done
						//child->SetOnOpen(true);	
					}
				}
				//It's on the open/closed list already with a lower cost, so don't do anything
			} else {
				//It's not on any list yet, so add it to the open list
				child->SetParent(best);
				child->SetCost(cost);
				child->SetTotal(total);
				
				m_storage->PushOntoOpen(child);
				child->SetOnOpen(true);
			}
		}
	}

	//Put the best node on the closed list, since we're done
	m_storage->PushOntoClosed(best);
	best->SetOnClosed(true);
}
