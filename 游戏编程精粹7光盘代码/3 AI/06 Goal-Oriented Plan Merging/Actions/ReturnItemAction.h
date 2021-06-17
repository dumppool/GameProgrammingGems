#ifndef RETURNITEMACTION_H
#define RETURNITEMACTION_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Action.h"
#include "../WorldProperty.h"
#include "../Agent.h"

namespace AI
{
	class ReturnItemAction : public Action
	{
	public:
		ReturnItemAction();
		ReturnItemAction(WorldPoint location);
		ReturnItemAction(const ReturnItemAction &rhs);
		virtual ~ReturnItemAction();
		ReturnItemAction* Clone() const;

		static const std::string m_name;
		virtual ActionResult Activate(Agent &agent, float &timeInAction);
	protected:
		WorldPoint m_location;
		mutable std::string m_nameAndLocation;
	public:
		virtual const std::string& GetName() const;
	
		void SetLocation(WorldPoint location);
		WorldPoint GetLocation() const;
	};
}

#endif //GOTO_H