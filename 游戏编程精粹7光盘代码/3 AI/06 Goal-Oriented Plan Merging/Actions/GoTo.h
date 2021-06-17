//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef GOTO_H
#define GOTO_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Action.h"
#include "../WorldProperty.h"
#include "../Agent.h"

namespace AI
{
	class GoTo : public Action
	{
	public:
		GoTo();
		GoTo(WorldPoint location);
		GoTo(const GoTo &rhs);
		virtual ~GoTo();
		GoTo* Clone() const;

		static const std::string m_name;
		virtual ActionResult Activate(Agent &agent, float &timeInAction);
	protected:
		WorldPoint m_location;
		mutable std::string m_nameAndLocation;
	public:
		virtual const std::string& GetName() const;
	
		virtual void SetLocation(WorldPoint location);
		WorldPoint GetLocation() const;
	};
}

#endif //GOTO_H