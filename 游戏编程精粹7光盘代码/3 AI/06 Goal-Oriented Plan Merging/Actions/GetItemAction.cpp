//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe


#include "GetItemAction.h"

#include "../global.h"
#include "../debug.h"
#include "../Agent.h"

using namespace AI;

#include "../World.h"
extern World g_world;

const std::string GetItemAction::m_name = "GetItemAction";

GetItemAction::GetItemAction() : m_itemLocation(0,0)
{
	WorldProperty getItemEffect(kHasItem, eBool);
	getItemEffect.m_value.m_bValue = true;

	m_effects.push_back(getItemEffect);

	WorldProperty getItemPrecondition(kAtLocation, eBool);
	getItemPrecondition.m_value.m_bValue = true;
	getItemPrecondition.m_location = m_itemLocation;

	m_preconditions.push_back(getItemPrecondition);
}
GetItemAction::GetItemAction(const GetItemAction &rhs) : Action(rhs) {
	m_itemLocation = rhs.m_itemLocation;
}
GetItemAction::~GetItemAction() {}
GetItemAction* GetItemAction::Clone() const {
	return new GetItemAction(*this);
}

const std::string& GetItemAction::GetName() const
{
	return m_name;
}

ActionResult GetItemAction::Activate(Agent &agent, float &timeInAction)
{
	WorldPoint location = WorldPoint(agent.GetXPos(), agent.GetYPos());
	g_world.RemoveItem(location);
	agent.AddItem(location);
	return ACTION_COMPLETE;
}

void GetItemAction::SetItemLocation(WorldPoint location) {
	m_itemLocation = location;
	m_preconditions[0].m_location = location;  ///This assumes that there's only one precondition; okay for our example
}
WorldPoint GetItemAction::GetItemLocation() {
	return m_itemLocation;
}
