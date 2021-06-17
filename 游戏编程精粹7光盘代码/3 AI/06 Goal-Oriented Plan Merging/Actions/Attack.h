//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef ATTACK_H
#define ATTACK_H

#ifdef _MSC_VER
#pragma once
#endif

#include "Action.h"

namespace AI {
	class Attack : public Action
	{
	public:
		static const std::string m_name;

	protected:
		Attack();
		virtual ~Attack();
		virtual ActionResult Activate();

		virtual const PropertyList& GetPreconditions() const;
		virtual const PropertyList& GetEffects() const;
		virtual const ContextPreconditionContainer& GetContextPreconditions() const;
	public:
		static Attack* Instance();		
		virtual const std::string& GetName() const;
	};

}

#endif //ATTACK_H