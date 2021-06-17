//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "ContextPrecondition.h"
#include "../Agent.h"

using namespace AI;

ContextPrecondition::ContextPrecondition()  {}
ContextPrecondition::~ContextPrecondition() {}

bool ContextPrecondition::IsPreconditionMet(const Agent *agent) const
{
	return false;
}
