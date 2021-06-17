//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef AI_WORKINGMEMORYFACT_H
#define AI_WORKINGMEMORYFACT_H

#ifdef _MSC_VER
#pragma once
#endif

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9math.h>

namespace AI {
	const float MEMORY_DECAY_RATE = 0.99f;

	template <typename T>
	class Attribute {
	public:
		T		m_value;
		float	m_confidence;
	};

	class WorkingMemoryFact
	{

	public:
		Attribute<long>	m_object;
		Attribute<D3DXVECTOR3>	m_position;

		inline void Decay() {
			m_position.m_confidence *= MEMORY_DECAY_RATE;
		}
	};
}

#endif
