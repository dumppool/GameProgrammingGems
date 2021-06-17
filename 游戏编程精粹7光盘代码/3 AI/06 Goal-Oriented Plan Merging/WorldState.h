//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#ifdef _MSC_VER
#pragma once
#endif

#include "WorldProperty.h"

namespace AI {
	class WorldState 
	{
	public:
		WorldState();
		~WorldState();

	private:
		PropertyList m_properties;
	};
}

#endif //WORLDSTATE_H