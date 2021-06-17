//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include <algorithm>
#include <sstream>

#include "Agent.h"
#include "Actions/ContextPrecondition.h"

#include "AStar/AStar.h"
#include "AStar/PriorityQueueStorage.h"

#include "Plan.h"

#include "global.h"
#include "world.h"

extern World g_world;

using namespace AI;
using namespace Lua;

//Returns the WorkingMemoryFact in this agent's memory that has to do with the ObjHandle passed in
// If such a WorkingMemoryFact doesn't exist, this function creates one, adds it to the agent's
// memory, and returns it
WorkingMemoryFact& FactMemory::GetFactById(const long id) {
	FactIterator i = m_facts.begin();
	for( ; i != m_facts.end(); ++i) {
		if(i->m_object.m_value == id) {
			return *i;
		}
	}

	WorkingMemoryFact fact;
	fact.m_object.m_value = id;
	fact.m_object.m_confidence = 1.0f;
	m_facts.push_back(fact);

	return m_facts.back();
}


///////////////////////////////////// AGENT ///////////////////////////////////
Agent::Agent(ActionContainer& actions, GoalContainer& goals, AgentType type, Team team) :	
											m_allValidActions(actions),
											m_allGoals(goals),
											m_team(team),
											m_hasBullets(false),
											m_hasMissiles(true),
											m_currentAction(NULL),
											m_timeInAction(0.0f),
											m_type(type),
											m_shouldReplan(true),
											m_hasAttacked(false),
											m_lastGoal(NULL),
											m_xPos(0),
											m_yPos(0)
{
	m_failedGoals.clear();
}

Agent::~Agent() {
	GoalContainer::iterator	goalIter = m_allGoals.begin();
	for( ; goalIter != m_allGoals.end(); ++goalIter) {
		delete *goalIter;
	}
}

void Agent::Load(LuaWrapper *lua, std::string agentTable) {
	float tempFloat;
	int numPatrolPoints;

	lua->GetFloatValueFromTable(tempFloat, "world", agentTable.c_str(), "startingPoint", "x");
	m_xPos = static_cast<int>(tempFloat);

	lua->GetFloatValueFromTable(tempFloat, "world", agentTable.c_str(), "startingPoint", "y");
	m_yPos = static_cast<int>(tempFloat);

	lua->GetFloatValueFromTable(tempFloat, "world", agentTable.c_str(), "numPatrolPoints");
	numPatrolPoints = static_cast<int>(tempFloat);

	for(int i = 0; i < numPatrolPoints; ++i) {
		std::stringstream patrolPoint;
		patrolPoint << "patrolPoint" << i;
		int x, y;

		lua->GetFloatValueFromTable(tempFloat, "world", agentTable.c_str(), patrolPoint.str().c_str(), "x");
		x = static_cast<int>(tempFloat);
		lua->GetFloatValueFromTable(tempFloat, "world", agentTable.c_str(), patrolPoint.str().c_str(), "y");
		y = static_cast<int>(tempFloat);

		AddPatrolPoint(std::pair<int,int>(x,y));
	}

	ResetPatrolPointIterator();
}

ActionDictionary* Agent::GetActionDictionary() {
	return &m_allValidActions;
}
GoalContainer* Agent::GetAllGoals() {
	return &m_allGoals;
}
Plan* Agent::GetCurrentPlan() {
	return &m_plan;
}
Plan* Agent::GetSecondaryPlan() {
	return &m_secondPlan;
}

const ActionContainer& Agent::GetValidActionsForProperty(const WorldProperty &prop)
{
	m_currentValidActions = m_allValidActions.GetActionsForProperty(prop);
	
	ActionContainer::iterator i = m_currentValidActions.begin();
	while(i != m_currentValidActions.end())
	{
		ContextPreconditionContainer contextPreconditions = (*i)->GetContextPreconditions();
		
		ContextPreconditionContainer::iterator contextPreconditionsIter = contextPreconditions.begin();
		bool erase = false;
		for( ; contextPreconditionsIter != contextPreconditions.end(); ++contextPreconditionsIter)
		{
			//Check each context precondition for truth. If any are false, we should delete this
			// action from the valid actions list.
			if(! ((*contextPreconditionsIter)->IsPreconditionMet(this)))
			{
				erase = true;
				break;
			}
		}
		if(erase == true)
		{
			m_currentValidActions.erase(i);
			if(m_currentValidActions.size() == 0) 
			{
				break;
			}
			i = m_currentValidActions.begin(); //TODO: this is horribly inefficient, but we're probably dealing with small lists
		} else {
			++i;
		}
	}

	return m_currentValidActions;
}

void Agent::SetXPos(unsigned int pos) {
	m_xPos = pos;
}
void Agent::SetYPos(unsigned int pos) {
	m_yPos = pos;
}
int Agent::GetXPos() const {
	return m_xPos;
}
int Agent::GetYPos() const {
	return m_yPos;
}

void Agent::SetHasMissiles(const bool missiles)
{
	m_hasMissiles = missiles;
}

bool Agent::HasMissiles() const
{
	return m_hasMissiles;
}

void Agent::SetHasBullets(const bool bullets)
{
	m_hasBullets = bullets;
}

bool Agent::HasBullets() const
{
	return m_hasBullets;
}

void Agent::SetHasAttacked(const bool attacked) {
	m_hasAttacked = attacked;
}
bool Agent::HasAttacked() {
	return m_hasAttacked;
}

void Agent::ResetActionsForSearch()
{
	m_allValidActions.ResetActionsForSearch();
}

bool Agent::HasCurrentPlan()
{
	return !m_plan.IsEmpty();
}

bool Agent::ShouldReplan() {
	return m_shouldReplan;
}
void Agent::SetShouldReplan(bool replan) {
	m_shouldReplan = replan;
	//If we're replanning, something's changed. We should reconsider previously failed goals
	m_failedGoals.clear();
}

void Agent::ClearCurrentAction() {
	m_currentAction = NULL;
}

Team Agent::GetTeam() const {
	return m_team;
}

void Agent::SetCurrentTargetLocation(WorldPoint location) {
	m_currentTargetLocation = location;
}
WorldPoint Agent::GetCurrentTargetLocation() const {
	return m_currentTargetLocation;
}

//Returns true if a plan is found.
// Could return false if there's no possible resolution for this goal
bool Agent::MakePlan(Goal *goal, Plan *plan)
{
	static AStar astar;
	static PriorityQueueStorage storage;

	astar.SetupSearch(goal, this, &storage);
	while(astar.IsSearchFinished() == false)
	{
		astar.Search();
	}

	if(astar.IsPathFound()) 
	{
		plan->CreatePlan(reinterpret_cast<Action*>(astar.GetResult()));
		return true;
	} else {
		return false;
	}
}

void Agent::ExecutePlan()
{
	if(m_currentAction == NULL)
	{
		m_currentAction = m_plan.GetNextAction();
		m_timeInAction = 0.0f;
	}
	if(m_currentAction != NULL)
	{
		m_timeInAction += FRAME_TIME;	
		m_currentActionResult = m_currentAction->Activate(*this, m_timeInAction);
		if(m_currentActionResult == ACTION_COMPLETE) {
			m_currentAction = m_plan.GetNextAction();
			m_timeInAction = 0.0f;
		}
	}
}

void Agent::NextPatrolPoint()
{
	++m_patrolPointIterator;
	if(m_patrolPointIterator == m_patrolPoints.end())
	{
		m_patrolPointIterator = m_patrolPoints.begin();
	}
}
WorldPoint Agent::GetCurrentPatrolPoint() {
	return *m_patrolPointIterator;
}
void Agent::ResetPatrolPointIterator() {
	m_patrolPointIterator =	m_patrolPoints.begin();
}


//Return "true" if it has detected a "significant" change
// "Significant" is, of course, relative to the sensor
bool Agent::UpdateSensors(const long id) {
	bool returnValue = false;

	//ObjHandle aiCompHandle    = this->GetAiComponentHandle();
	//AIComponent *aiComponent  = g_aiManager->GetAIComponent(aiCompHandle);
	//long id     = aiComponent->GetGameObjectHandle();
	//GameObject *agentObject   = g_resourceManager->GetGameObject(agentHandle);

	return returnValue;
}

void Agent::DecayMemory() {
	m_memory.DecayMemory();
}
void FactMemory::DecayMemory() {
	FactIterator i = m_facts.begin();
	for( ; i != m_facts.end(); ++i) {
		i->Decay();
	}
}

Goal* Agent::GetBestGoal() {
	std::sort(m_allGoals.begin(), m_allGoals.end(), GoalPriorityGreater());
	return m_allGoals.front();
}
Goal* Agent::GetSecondGoal() {
	if(m_allGoals.size() == 1) {
		return m_allGoals.front();
	}
	return m_allGoals[1];
}
Goal* Agent::GetLastGoal() {
	return m_lastGoal;
}
void Agent::SetLastGoal(Goal *goal) {
	m_lastGoal = goal;
}

void Agent::FailGoal(Goal *goal) {
	m_failedGoals.push_back(goal);
}

void Agent::Die() {}

void Agent::AddPatrolPoint(WorldPoint point) {
	m_patrolPoints.push_back(point);
}

void Agent::Update() {
	bool newGoal = false;
	if(m_shouldReplan || !HasCurrentPlan()) {
		//Check to see which goal is most important to the agent right now.
		// If the most important goal hasn't changed, don't bother to replan.
		// Note this means if a plan fails, we'll need some way of ensuring the agent doesn't continually attempt the same
		// (failing) plan
		bool hasPlan = false;
		do {
			Goal* agentGoal = GetBestGoal();
			if(agentGoal != GetLastGoal() || !HasCurrentPlan()) {
				if(!MakePlan(agentGoal, &m_plan)) {
					//We completely failed to make a plan for this goal
					FailGoal(agentGoal);
				} else {
					//We've got a new plan. Update what goal we're pursuing				
					SetLastGoal(agentGoal);
					newGoal = true;
					hasPlan = true;
				}
			} else {
				//If we're here, our previous plan should still be valid.
				hasPlan = true;
			}
		} while (!hasPlan);
		SetShouldReplan(false);
	}

	//We have a plan. Execute it
	if(newGoal) {
		if(g_world.IsPlanMergingOn()) {
			//make a plan for second-most-important goal
			MakePlan(GetSecondGoal(), &m_secondPlan);
			bool merged = false;
			Plan mergedPlan = Plan::MergePlans(m_plan, m_secondPlan, merged);
			if(merged) {
				m_plan = mergedPlan;
			}
		}
		//dprintf("Executing plan for %s", m_agent.GetLastGoal()->GetName().c_str());
		ClearCurrentAction();
	}
	ExecutePlan();
}

void Agent::AddItem(WorldPoint location) {
	m_items.push_back(location);
}
WorldPoint Agent::DropItem() {
	WorldPoint value = m_items.back();
	m_items.pop_back();
	return value;
}
bool Agent::HasItems() const {
	return !m_items.empty();
}