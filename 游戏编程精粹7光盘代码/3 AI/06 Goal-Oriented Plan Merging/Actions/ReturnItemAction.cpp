#include "ReturnItemAction.h"

#include <algorithm>

#include "../World.h"
extern World g_world;

using namespace AI;

const std::string ReturnItemAction::m_name = "ReturnItemAction";

ReturnItemAction::ReturnItemAction() : m_location(-1,-1)
{
	WorldProperty returnItemEffect(kReturnedItem, eBool);
	returnItemEffect.m_value.m_bValue = true;
	m_effects.push_back(returnItemEffect);

	WorldProperty returnItemPrecondition(kHasItem, eBool);
	returnItemPrecondition.m_value.m_bValue = true;
	m_preconditions.push_back(returnItemPrecondition);
}
ReturnItemAction::ReturnItemAction(WorldPoint location) {
	WorldProperty returnItemEffect(kReturnedItem, eBool);
	returnItemEffect.m_value.m_bValue = true;
	m_effects.push_back(returnItemEffect);

	WorldProperty returnItemPrecondition(kHasItem, eBool);
	returnItemPrecondition.m_value.m_bValue = true;
	m_preconditions.push_back(returnItemPrecondition);

	m_location = location;
}
ReturnItemAction::ReturnItemAction(const ReturnItemAction &rhs) : Action(rhs) {
	m_location = rhs.m_location;
}
ReturnItemAction::~ReturnItemAction() {}
ReturnItemAction* ReturnItemAction::Clone() const {
	return new ReturnItemAction(*this);
}

void ReturnItemAction::SetLocation(WorldPoint location) {
	m_location = location;
}
WorldPoint ReturnItemAction::GetLocation() const {
	return m_location;
}

const std::string& ReturnItemAction::GetName() const
{
	return m_name;
}

ActionResult ReturnItemAction::Activate(Agent &agent, float &timeInAction)
{
	if(agent.GetXPos() != m_location.first) {
		if(m_location.first > agent.GetXPos()) {
			agent.SetXPos(agent.GetXPos() + 1);
		} else {
			agent.SetXPos(agent.GetXPos() - 1);
		}
		return ACTION_IN_PROGRESS;
	} else if(agent.GetYPos() != m_location.second) {
		if(m_location.second > agent.GetYPos()) {
			agent.SetYPos(agent.GetYPos() + 1);
		} else {
			agent.SetYPos(agent.GetYPos() - 1);
		}
		return ACTION_IN_PROGRESS;
	} else {
		//Return the items to base
		while(agent.HasItems()) {

			WorldPoint itemLocation = agent.DropItem();

			//Remove the "getItem" goal for this item
			GoalContainer *goals = agent.GetAllGoals();
			delete goals->front();
			goals->erase(goals->begin());

			break;

		}

		return ACTION_COMPLETE;
	}
}
