#include "World.h"
#include "GLinc.h"
#include "global.h"
#include "debug.h"

#include <algorithm>
#include <sstream>

#include "Actions/ActionDictionary.h"
#include "Goals/GetItemGoal.h"


#include "AgentFactory.h"

using namespace Lua;

//GLFont written by Brad Fish
//  bhf5@email.byu.edu
#include "GLFont2.h"
using namespace glfont;
extern GLFont g_font;

using namespace AI;

World::World() : m_rows(0), m_cols(0), m_blueBase(0,0), m_redBase(-1,-1), m_mergePlans(false)
{}
World::~World() {
	AgentContainer::iterator iter = m_agents.begin();
	for( ; iter != m_agents.end(); ++iter) {
		delete *iter;
	}

	m_agents.clear();
}

const int World::GetCols() const {
	return m_cols;
}
const int World::GetRows() const {
	return m_rows;
}

void World::TogglePlanMerge() {
	m_mergePlans = !m_mergePlans;
}
bool World::IsPlanMergingOn() {
	return m_mergePlans;
}

void World::GenerateItems() {
	//If there are already items in the world, or the agent is already busy with items, don't generate more
	// (I didn't build a real agent perception module for this demo, so the agent can get confused by
	//  items appearing in the middle of his item-collection plan)
	if(m_items.empty() && m_agents[0]->GetAllGoals()->size() == 1) {
		int x,y;
		x = rand() % GetCols();
		y = rand() % GetRows();
		AddItem(WorldPoint(x,y));

		x = rand() % GetCols();
		y = rand() % GetRows();
		AddItem(WorldPoint(x,y));
	}
}

void World::Load(LuaWrapper *lua) {
	float tempFloat;
	std::string tempString;
	int numAgents;

	lua->GetFloatValueFromTable(tempFloat, "world", "numRows");
	m_rows = static_cast<int>(tempFloat);

	lua->GetFloatValueFromTable(tempFloat, "world", "numCols");
	m_cols = static_cast<int>(tempFloat);

	lua->GetFloatValueFromTable(tempFloat, "world", "numAgents");
	numAgents = static_cast<int>(tempFloat);
	for(int i = 0; i < numAgents; ++i) {
		std::stringstream agentIndex;
		std::string type, team;
		agentIndex << "agent" << i;
		lua->GetStringValueFromTable(type, "world", agentIndex.str().c_str(), "type");
		lua->GetStringValueFromTable(team, "world", agentIndex.str().c_str(), "team");
		if(type == "patrol") {
			if(team == "blue") {
				m_agents.push_back(AgentFactory::Instance()->CreateAgent(AGENT_PATROL, TEAM_BLUE));
			} else {
				m_agents.push_back(AgentFactory::Instance()->CreateAgent(AGENT_PATROL, TEAM_RED));
			}
		} else if (type == "none") {
			if(team == "blue") {
				m_agents.push_back(AgentFactory::Instance()->CreateAgent(AGENT_NONE, TEAM_BLUE));
			} else {
				m_agents.push_back(AgentFactory::Instance()->CreateAgent(AGENT_NONE, TEAM_RED));
			}
		}
		Agent *agent = m_agents.back();
		agent->Load(lua, agentIndex.str());
	}

	for(unsigned int i = 0; i < m_rows; ++i) {
		std::vector<TerrainType> tempTerrain;
		for(unsigned int j = 0; j < m_cols; ++j) {
			tempTerrain.push_back(eNormal);
		}
		m_terrain.push_back(tempTerrain);
	}

}


void World::Update() {
	AgentContainer::iterator iter = m_agents.begin();
	for( ; iter != m_agents.end(); ++iter) {
		(*iter)->Update();
	}
}

void World::Draw(int width, int height) {
	int rectColumnWidth = width/m_cols;
	int rectRowHeight	= height/m_rows; 

	int currentWidth  = 0;
	int currentHeight = 0;
	
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_ONE,	GL_ZERO);
	glColor3f(0.0F, 0.0F, 0.0F);

	glBegin(GL_LINES);
	for(int i = 0; i <= width; i = i + rectColumnWidth) {
		for(int j = 0; j <= height; j = j + rectRowHeight) {
			glVertex2i(0, j);
			glVertex2i(width,j);
			
			glVertex2i(i, 0);
			glVertex2i(i, height);
		}
	}
	glEnd();

	//Draw bases
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2i( m_blueBase.first * rectColumnWidth, m_blueBase.second * rectRowHeight);
	glVertex2i((m_blueBase.first + 1) * rectColumnWidth , m_blueBase.second * rectRowHeight);
	glVertex2i((m_blueBase.first + 1) * rectColumnWidth, (m_blueBase.second + 1) * rectRowHeight);
	glVertex2i( m_blueBase.first * rectColumnWidth, (m_blueBase.second + 1)* rectRowHeight);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2i( m_redBase.first * rectColumnWidth, m_redBase.second * rectRowHeight);
	glVertex2i((m_redBase.first + 1) * rectColumnWidth , m_redBase.second * rectRowHeight);
	glVertex2i((m_redBase.first + 1) * rectColumnWidth, (m_redBase.second + 1) * rectRowHeight);
	glVertex2i( m_redBase.first * rectColumnWidth, (m_redBase.second + 1)* rectRowHeight);
	glEnd();

	int x, y, centerX, centerY;

	//If the item is in the world, draw it
	ItemContainer::iterator itemIter = m_items.begin();
	for( ; itemIter != m_items.end(); ++itemIter) {
		glColor3f(1.0f, 0, 0);

		x = itemIter->first;
		y = itemIter->second;

		centerX = (x * rectColumnWidth) + (rectColumnWidth / 2);
		centerY = (y * rectRowHeight) + (rectRowHeight / 2);

		glBegin(GL_QUADS);
		glVertex2i(centerX - (rectColumnWidth / 6), (y * rectRowHeight));
		glVertex2i(centerX - (rectColumnWidth / 6), ((y+1) * rectRowHeight));
		glVertex2i(centerX + (rectColumnWidth / 6), ((y+1) * rectRowHeight));
		glVertex2i(centerX + (rectColumnWidth / 6), (y * rectRowHeight));
	
		glVertex2i((x * rectColumnWidth),		centerY - (rectRowHeight / 6));
		glVertex2i(((x+1) * rectColumnWidth),	centerY - (rectRowHeight / 6));
		glVertex2i(((x+1) * rectColumnWidth),	centerY + (rectRowHeight / 6));
		glVertex2i((x * rectColumnWidth),		centerY + (rectRowHeight / 6));

		glEnd();
	}


	AgentContainer::iterator iter = m_agents.begin();
	float angle, radius;
	radius = rectColumnWidth / 3.0f;
	for( ; iter != m_agents.end(); ++iter) {
		if((*iter)->GetTeam() == TEAM_RED) {
			glColor3f(1.0f, 0.0f, 0.0f);
		} else {
			glColor3f(0.0f, 0.0f, 1.0f);
		}

		x = (*iter)->GetXPos();
		y = (*iter)->GetYPos();

		centerX = (x * rectColumnWidth) + (rectColumnWidth / 2);
		centerY = (y * rectRowHeight) + (rectRowHeight / 2);

		//This is a bad, but simple, way to draw a circle
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(centerX, centerY, 1.0f);
		for(int i = 0; i <= 100; ++i) {
			angle = 2.0f * PI * ((float)i / 100.0f);
			glVertex3f(radius * cos(angle) + centerX , radius * sin(angle) + centerY, 0.0f);
		}
		glEnd();
	}
}

//That's "Goals, Plan, and Actions," rather than grade-point average =b
void World::DrawGPA(int agentId, int left, int top) {
	//float top_color[3] = {1.0F, 1.0F, 1.0F};
	//float bottom_color[3] = {0.0F, 0.0F, 1.0F};

	const int lineHeight = 12; //varies with font
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA,	GL_ONE_MINUS_SRC_ALPHA);

	Agent* agent = m_agents[agentId];

	left += 5; //get off the grid a little


	std::string agentName = "Agent ";
	agentName += agentId;

	glLoadIdentity();
	//Draw a string
	glColor3f(0.0F, 0.0F, 0.0F);

	g_font.Begin();

	
	//Draw text for title & instuctions
	g_font.DrawString("Plan Merging Demo", 1.0f, left, top);
	top -= lineHeight * 2;
	g_font.DrawString("Press 'p' to turn ", 1.0f, left, top);
	top -= lineHeight;
	g_font.DrawString(" plan merging on/off", 1.0f, left, top);
	top -= lineHeight;
	g_font.DrawString("  Plan meging changes will ", 1.0f, left, top);
	top -= lineHeight;
	g_font.DrawString("  affect next replan.", 1.0f, left, top);
	top -= lineHeight * 2;

	g_font.DrawString("Press 'i' to create items.", 1.0f, left, top);
	top -= lineHeight;
	g_font.DrawString(" Items can only be created", 1.0f, left, top);
	top -= lineHeight;
	g_font.DrawString(" a pair at a time, and only", 1.0f, left, top);
	top -= lineHeight;
	g_font.DrawString(" during a patrol plan", 1.0f, left, top);
	top -= lineHeight * 4;


	g_font.DrawString("Status:", 1.0f, left, top);
	top -= lineHeight;

	if(m_mergePlans) {
		g_font.DrawString("Plan merging is on", 1.0f, left, top);
	} else {
		g_font.DrawString("Plan merging is off", 1.0f, left, top);
	}
	top -= lineHeight * 2;

	g_font.DrawString("Goals for " + agentName, 1.0F, left, top);
	top -= lineHeight;

	GoalContainer *agentGoals = agent->GetAllGoals();
	GoalContainer::iterator goalIter = agentGoals->begin();
	for( ; goalIter != agentGoals->end(); ++goalIter) {
		std::string goalLine = (*goalIter)->GetName();
		goalLine += ": ";
		goalLine += (*goalIter)->GetPriority();
		g_font.DrawString(goalLine, 1.0F, left, top);
		top -= lineHeight;
	}

	top -= lineHeight;

	g_font.DrawString("All available actions ", 1.0F, left, top);
	top -= lineHeight;
	g_font.DrawString("    for " + agentName, 1.0F, left, top);
	top -= lineHeight;
	
	ActionMap *agentActions = agent->GetActionDictionary()->GetAllActions();
	ActionMap::iterator actionIter = agentActions->begin();
	for( ; actionIter != agentActions->end(); ++actionIter) {
		g_font.DrawString(actionIter->second->GetName(), 1.0f, left, top);
		top -= lineHeight;
	}

	top -= lineHeight;

	g_font.DrawString("Current plan for " + agentName, 1.0F, left, top);
	top -= lineHeight;

	Plan *agentPlan = agent->GetCurrentPlan();
	std::string planString = agentPlan->GetPlanString();
	
	std::string::size_type stringPos = 0;

	do {
		planString = planString.substr(stringPos, planString.size());
		stringPos = planString.find_first_of("\n", 1);
		g_font.DrawString(planString.substr(0, stringPos), 1.0f, left, top);
		top -= lineHeight;
	} while(stringPos != std::string::npos);

}

void World::AddItem(WorldPoint location) {
	m_items.push_back(location);

	//Tell the agent (via the goal) there's an item
	GetItemGoal *newGoal = new GetItemGoal(location);
	newGoal->SetPriority(0.5);

	GoalContainer *goals = m_agents[0]->GetAllGoals();
	goals->push_back(newGoal);
	m_agents[0]->SetShouldReplan(true);
}
void World::RemoveItem(WorldPoint location) {
	ItemContainer::iterator itemIter = std::find(m_items.begin(), m_items.end(), location);
	if(itemIter != m_items.end()) {
		m_items.erase(itemIter);
	}
}
const int World::GetNumItems() const {
	return m_items.size();
}
WorldPoint World::GetItemLocation(int index) const {
	return m_items[index];
}

WorldPoint World::GetBaseLocation(AI::Team team) const {
	if(team == TEAM_BLUE) {
		return m_blueBase;
	} else {
		return m_redBase;
	}
}

