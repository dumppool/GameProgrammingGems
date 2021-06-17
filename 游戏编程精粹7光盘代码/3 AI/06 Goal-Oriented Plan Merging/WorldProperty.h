//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#ifndef WORLDPROPERTY_H
#define WORLDPROPERTY_H

#ifdef _MSC_VER
#pragma once
#endif

//More or less taken from Jeff Orkin's article in AI Wisdom 2, section 3.4

#include <vector>

namespace AI {
	class WorldProperty;
	typedef std::vector<WorldProperty> PropertyList;

	typedef std::pair<int, int> WorldPoint;

	enum WorldPropertyKey 
	{
		kTargetIsDead,
		kTargetInMissileRange,
		kTargetInGunRange,
		kAtLocation,
		kPatrolling,
		kHasItem,
		kReturnedItem,
		kBelowMinimumAltitude,
		kProvideCover,
		kDie
	};
	enum WorldPropertyValueType
	{
		eBool,
		eFloat,
		eInt,
		eLocation
	};
	enum LocationType
	{
		eLocationNothing = 0,
		eLocationCover
	};

	class WorldProperty
	{
	public:
		WorldProperty(WorldPropertyKey key, WorldPropertyValueType type);
		~WorldProperty();

		int m_objectId;  //This would be whatever type your game object handles are
		WorldPropertyKey		m_key;
		WorldPropertyValueType	m_type;

		union value
		{
			bool  m_bValue;
			float m_fValue;
			int   m_nValue;
		} m_value;

		WorldPoint m_location;

		bool operator==(const WorldProperty &rhs) const;
	};
}
#endif //WORLDPROPERTY_H