//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "Attack.h"

using namespace AI;

const std::string Attack::m_name = "Attack";

Attack::Attack()
{
	WorldProperty attackEffect(kTargetIsDead, eBool);
	attackEffect.m_value.m_bValue	= true;

	m_effects.push_back(attackEffect);
}

Attack::~Attack() {}

Attack* Attack::Instance() {
	static Attack m_attack;
	return &m_attack;
}

const std::string& Attack::GetName() const 
{
	return m_name;
}

const PropertyList& Attack::GetPreconditions() const
{
	return m_preconditions;
}

const PropertyList& Attack::GetEffects() const
{
	return m_effects;
}

const ContextPreconditionContainer& Attack::GetContextPreconditions() const
{
	return m_contextPreconditions;
}

ActionResult Attack::Activate()
{
	return ACTION_FAILED;
}
