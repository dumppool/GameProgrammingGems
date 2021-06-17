#ifndef __AISHOOTER_AGENT_H__
#define __AISHOOTER_AGENT_H__

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <DataSet.h>
#include <FeatureMap.h>

class Ship;

/**
 * A behavioral cloning agent. Currently supports Lua scripts.
 */
class Agent
{
public:
    /**
     * Initialize the agent.
     * \param ship the ship this agent will control
     */
    Agent (Ship* ship);
    ~Agent ();

    /**
     * Initialize the agent with a script.
     * \param script the filename of the script to load
     */
    void init (const std::string& script);

    /**
     * Act on the given game state.
     * \param state the current game state
     */
    void act (const DataSet::raw_row_t& state);
private:
    // Setup script callbacks
    static int setup_addInterval (lua_State *state);

    // Agent control script callbacks
    static int agent_accel (lua_State *state);
    static int agent_turn (lua_State *state);
    static int agent_fire (lua_State *state);

    lua_State *mLua;
    FeatureMap mFeatureMap;
    Ship *mShip;
};

#endif
