//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_ACTIONS_GETITEMACTION
#define AI_ACTIONS_GETITEMACTION

#ifdef _MSC_VER
#pragma once
#endif

#include "Action.h"
#include "../Agent.h"

namespace AI 
{
	class GetItemAction : public Action
	{
	public:
		GetItemAction();
		GetItemAction(const GetItemAction &rhs);
		virtual ~GetItemAction();
		GetItemAction* Clone() const;

		static const std::string m_name;
		virtual ActionResult Activate(Agent &agent, float &timeInAction);
	protected:
		WorldPoint m_itemLocation;
	public:
		virtual const std::string& GetName() const;

		void SetItemLocation(WorldPoint location);
		WorldPoint GetItemLocation();
	};
}

#endif //AI_ACTIONS_PATROL