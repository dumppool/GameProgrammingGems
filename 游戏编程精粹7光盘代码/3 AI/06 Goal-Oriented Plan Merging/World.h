#ifndef WORLD_H
#define WORLD_H

#ifdef _MSC_VER
#pragma once
#endif

#include <vector>

#include "Agent.h"
#include "LuaWrapper.h"

using namespace AI;

enum TerrainType {
	eNormal,
	eBase,
	eObstacle,
};

class World {
	typedef std::vector<Agent *> AgentContainer;
	typedef std::vector<WorldPoint> ItemContainer;
	typedef std::vector<std::vector<TerrainType> > TerrainTypeContainer;
public:
	World();
	~World();
	void Load(Lua::LuaWrapper *lua);

	void Update();
	void Draw(int width, int height);
	void DrawGPA(int agentId, int left, int top);

	const int GetCols() const;
	const int GetRows() const;

	const int GetNumItems() const;

	bool IsPlanMergingOn();
	void TogglePlanMerge();

	void GenerateItems();

	void AddItem(WorldPoint location);
	void RemoveItem(WorldPoint location);
	WorldPoint GetItemLocation(int index) const;

	WorldPoint GetBaseLocation(AI::Team team) const;
private:
	unsigned int m_rows, m_cols;
	TerrainTypeContainer m_terrain;
	ItemContainer	m_items;
	AgentContainer  m_agents;
	WorldPoint m_blueBase, m_redBase;

	bool m_mergePlans;
};

#endif
