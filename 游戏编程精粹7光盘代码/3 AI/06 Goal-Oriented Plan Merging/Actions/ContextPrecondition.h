//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef CONTEXTPRECONDITION_H
#define CONTEXTPRECONDITION_H

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>

namespace AI
{	
	class Agent;

	class ContextPrecondition;

	typedef std::vector<ContextPrecondition *> ContextPreconditionContainer;

	class ContextPrecondition
	{
	public:
		ContextPrecondition();
		virtual ~ContextPrecondition() = 0;

		virtual bool IsPreconditionMet(const Agent *agent) const;
	};
}

#endif  //CONTEXTPRECONDITION_H