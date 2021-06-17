#include <stdexcept>

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Common.h"
#include "Ship.h"
#include "Agent.h"

Agent::Agent (Ship* ship)
: mShip (ship), mLua (NULL)
{
    mFeatureMap.defineFeature ("opp_distance", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_dir_to", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_dir_from", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_vnorm", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_ddir_to", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_ddir_from", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_hp", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("opp_turning", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("opp_accel", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("opp_fire", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("my_hp", FeatureMap::FT_CONTINUOUS);
    mFeatureMap.defineFeature ("my_turning", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("my_accel", FeatureMap::FT_NOMINAL);
    mFeatureMap.defineFeature ("my_fire", FeatureMap::FT_NOMINAL);
}

Agent::~Agent ()
{
    if (mLua) {
        lua_close (mLua);
    }
}

void Agent::init (const std::string& script)
{
    mLua = lua_open ();

    luaL_openlibs (mLua);

    // Create "AIShooter" module
    lua_newtable (mLua);

    lua_pushlightuserdata (mLua, this);
    lua_pushcclosure (mLua, Agent::setup_addInterval, 1);
    lua_setfield (mLua, -2, "addInterval");

    lua_setglobal (mLua, "AIShooter");

    // Create "Agent" module
    lua_newtable (mLua);

    lua_pushlightuserdata (mLua, this);
    lua_pushcclosure (mLua, Agent::agent_accel, 1);
    lua_setfield (mLua, -2, "accel");

    lua_pushlightuserdata (mLua, this);
    lua_pushcclosure (mLua, Agent::agent_turn, 1);
    lua_setfield (mLua, -2, "turn");

    lua_pushlightuserdata (mLua, this);
    lua_pushcclosure (mLua, Agent::agent_fire, 1);
    lua_setfield (mLua, -2, "fire");

    lua_setglobal (mLua, "Agent");

    // Create empty "GameState" table
    lua_newtable (mLua);
    lua_setglobal (mLua, "GameState");

    if (luaL_dofile (mLua, script.c_str ())) {
        const char* error = lua_tostring (mLua, 1);
        throw std::runtime_error ("Unable to open file '"
            + script + "': " + error);
    }

    lua_getglobal (mLua, "setupFeatureMap");
    if (lua_pcall (mLua, 0, 0, 0)) {
        const char* err = lua_tostring (mLua, lua_gettop (mLua));
        throw std::runtime_error (err);
    }
}

void Agent::act (const DataSet::raw_row_t& state)
{
    DataSet::raw_row_t::const_iterator iter = state.begin (), end = state.end ();
    
    lua_getglobal (mLua, "GameState");

    for (;iter != end; iter ++) {
        lua_pushnumber (mLua, mFeatureMap.mapFeature (iter->first,
            iter->second));
        lua_setfield (mLua, -2, iter->first.c_str ());
    }

    lua_pop (mLua, 1);

    lua_getglobal (mLua, "accelTree");
    if (lua_pcall (mLua, 0, 0, 0)) {
        const char* err = lua_tostring (mLua, lua_gettop (mLua));
        throw std::runtime_error (err);
    }

    lua_getglobal (mLua, "turnTree");
    if (lua_pcall (mLua, 0, 0, 0)) {
        const char* err = lua_tostring (mLua, lua_gettop (mLua));
        throw std::runtime_error (err);
    }

    lua_getglobal (mLua, "fireTree");
    if (lua_pcall (mLua, 0, 0, 0)) {
        const char* err = lua_tostring (mLua, lua_gettop (mLua));
        throw std::runtime_error (err);
    }
}

int Agent::setup_addInterval (lua_State *state)
{
    assert (lua_islightuserdata (state, lua_upvalueindex (1)));

    Agent *agent = reinterpret_cast<Agent*> (
        lua_touserdata (state, lua_upvalueindex (1)));

    if(lua_gettop (state) != 4) {
        luaL_error (state,
            "There should be four arguments to AddInterval!");
    }

    if(!lua_isstring (state, 1)) {
        luaL_error (state,
            "The first argument to AddInterval should be a string!");
    }

    if(!lua_isnumber (state, 2)) {
        luaL_error (state,
            "The second argument to AddInterval should be a number!");
    }

    if(!lua_isnumber (state, 3)) {
        luaL_error (state,
            "The third argument to AddInterval should be a number!");
    }

    if(!lua_isnumber (state, 4)) {
        luaL_error (state,
            "The fourth argument to AddInterval should be a number!");
    }

    const char* feature = lua_tostring (state, 1);
    int value = (int) floor (lua_tonumber (state, 2) + 0.5f);
    float lower = (float) lua_tonumber (state, 3);
    float upper = (float) lua_tonumber (state, 4);

    agent->mFeatureMap.addInterval (feature, value, lower, upper);

    return 0;
}

int Agent::agent_accel (lua_State *state)
{
    assert (lua_islightuserdata (state, lua_upvalueindex (1)));

    Agent *agent = reinterpret_cast<Agent*> (
        lua_touserdata (state, lua_upvalueindex (1)));

    if(lua_gettop (state) != 1) {
        luaL_error (state,
            "There should be one argument to Accel!");
    }

    if(!lua_isstring (state, 1)) {
        luaL_error (state,
            "The first argument to accel should be a string!");
    }

    const char* accel = lua_tostring (state, 1);

    if (!strcmp (accel, "none")) {
        agent->mShip->accel (Ship::ACCEL_NONE);
    } else if (!strcmp (accel, "normal")) {
        agent->mShip->accel (Ship::ACCEL_NORMAL);
    } else if (!strcmp (accel, "reverse")) {
        agent->mShip->accel (Ship::ACCEL_REVERSE);
    } else {
        luaL_error (state, "Unknown accel parameter '%s'", accel);
    }

    return 0;
}

int Agent::agent_turn (lua_State *state)
{
    assert (lua_islightuserdata (state, lua_upvalueindex (1)));

    Agent *agent = reinterpret_cast<Agent*> (
        lua_touserdata (state, lua_upvalueindex (1)));

    if(lua_gettop (state) != 1) {
        luaL_error (state,
            "There should be one argument to turn!");
    }

    if(!lua_isstring (state, 1)) {
        luaL_error (state,
            "The first argument to turn should be a string!");
    }

    const char* turn = lua_tostring (state, 1);

    if (!strcmp (turn, "none")) {
        agent->mShip->turn (Ship::TURN_NONE);
    } else if (!strcmp (turn, "left")) {
        agent->mShip->turn (Ship::TURN_LEFT);
    } else if (!strcmp (turn, "right")) {
        agent->mShip->turn (Ship::TURN_RIGHT);
    } else {
        luaL_error (state, "Unknown turn parameter '%s'", turn);
    }

    return 0;
}

int Agent::agent_fire (lua_State *state)
{
    assert (lua_islightuserdata (state, lua_upvalueindex (1)));

    Agent *agent = reinterpret_cast<Agent*> (
        lua_touserdata (state, lua_upvalueindex (1)));

    if(lua_gettop (state) != 1) {
        luaL_error (state,
            "There should be one argument to fire!");
    }

    if(!lua_isstring (state, 1)) {
        luaL_error (state,
            "The first argument to fire should be a string!");
    }

    const char* fire = lua_tostring (state, 1);

    agent->mShip->fire (!strcmp(fire, "fire"));

    return 0;
}
