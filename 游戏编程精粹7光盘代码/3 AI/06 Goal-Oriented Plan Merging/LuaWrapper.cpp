//Copyright (c) 2006 Digipen (USA) Corp.
//Author: Michael Dawe

#include "LuaWrapper.h"

#include "debug.h"

#include <iostream>
#include <assert.h>
#include <cstdarg> // for variable argument lists

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9math.h>

using namespace Lua;

LuaWrapper::LuaWrapper() {}

LuaWrapper::~LuaWrapper() {
	lua_close(m_luaState);
}

//Lua functions
static int Panic(lua_State *L)
{
	dprintf("Fatal Lua Error: %s\n", lua_tostring(L, -1));
	lua_close(L);
	exit(-1);
	return 0;
}

int LuaWrapper::MakeVector2(lua_State *L)
{
	int numArguments = lua_gettop(L);
	assert(numArguments == 2);

	float first  = (float)lua_tonumber(L, 1);
	float second = (float)lua_tonumber(L, 2);

	//lua_newuserdata creates a new space in lua stack memory of whatever size we specify
	void* vector = lua_newuserdata(L, sizeof(D3DXVECTOR2) + sizeof(size_t) );

	//In the first sizeof(size_t) bytes, put the size of the rest of the data
	*((size_t*)vector) = sizeof(D3DXVECTOR2);

	//now move the pointer forward size_t bytes
	vector = ((size_t *)vector) + 1;

	//Populate the rest of the memory with our vector data
	((D3DXVECTOR2 *)vector)->x = first;
	((D3DXVECTOR2 *)vector)->y = second;

	//We've put one new thing on the stack (the new memory chunk)
	return 1;
}

int LuaWrapper::MakeVector3(lua_State *L)
{
	int numArguments = lua_gettop(L);
	assert(numArguments == 3);

	float first  = (float)lua_tonumber(L, 1);
	float second = (float)lua_tonumber(L, 2);
	float third	 = (float)lua_tonumber(L, 3);

	//lua_newuserdata creates a new space in lua stack memory of whatever size we specify
	void* vector = lua_newuserdata(L, sizeof(D3DXVECTOR3) + sizeof(size_t) );

	//In the first sizeof(size_t) bytes, put the size of the rest of the data
	*((size_t*)vector) = sizeof(D3DXVECTOR3);

	//now move the pointer forward size_t bytes
	vector = ((size_t *)vector) + 1;

	//Populate the rest of the memory with our vector data
	((D3DXVECTOR3 *)vector)->x = first;
	((D3DXVECTOR3 *)vector)->y = second;
	((D3DXVECTOR3 *)vector)->z = third;

	//We've put one new thing on the stack (the new memory chunk)
	return 1;
}

int LuaWrapper::MakeVector4(lua_State *L)
{
	int numArguments = lua_gettop(L);
	assert(numArguments == 4);

	float first  = (float)lua_tonumber(L, 1);
	float second = (float)lua_tonumber(L, 2);
	float third	 = (float)lua_tonumber(L, 3);
	float fourth = (float)lua_tonumber(L, 4);

	//lua_newuserdata creates a new space in lua stack memory of whatever size we specify
	void* vector = lua_newuserdata(L, sizeof(D3DXVECTOR4) + sizeof(size_t) );

	//In the first sizeof(size_t) bytes, put the size of the rest of the data
	*((size_t*)vector) = sizeof(D3DXVECTOR4);

	//now move the pointer forward size_t bytes
	vector = ((size_t *)vector) + 1;

	//Populate the rest of the memory with our vector data
	((D3DXVECTOR4 *)vector)->x = first;
	((D3DXVECTOR4 *)vector)->y = second;
	((D3DXVECTOR4 *)vector)->z = third;
	((D3DXVECTOR4 *)vector)->w = fourth;

	//We've put one new thing on the stack (the new memory chunk)
	return 1;
}

void LuaWrapper::Initialize() {
	//initialize the lua state
	m_luaState = lua_open();

	lua_atpanic(m_luaState, &::Panic);

	//load lua libraries
//	luaopen_io(m_luaState); 
    luaopen_base(m_luaState);
    luaopen_table(m_luaState);
    luaopen_string(m_luaState);
    luaopen_math(m_luaState);

	lua_register(m_luaState, "MakeVector2", &Lua::LuaWrapper::MakeVector2);
	lua_register(m_luaState, "MakeVector3", &Lua::LuaWrapper::MakeVector3);
	lua_register(m_luaState, "MakeVector4", &Lua::LuaWrapper::MakeVector4);

	int status = luaL_loadfile(m_luaState, "Lua Scripts/main.lua");
	
	if(status != 0) {
		dprintf("-- %s\n", lua_tostring(m_luaState, -1));

		assert(false && "Lua error; likely a parse error. Check output for further info");
		dprintf("-- %s\n", lua_tostring(m_luaState, -1));
		lua_pop(m_luaState, 1); // remove error message
	}
	else if(status == 0) {
		status = lua_pcall(m_luaState, 0, LUA_MULTRET, 0);
	}

}

lua_State* LuaWrapper::GetLuaState() {
	return m_luaState;
}

LuaWrapper* LuaWrapper::Instance() {
	static LuaWrapper _luaWrapper;
	return &_luaWrapper;
}

HRESULT LuaWrapper::TraverseTable(const char *tableName) {
	lua_getglobal(m_luaState, tableName);

	if(!lua_istable(m_luaState, -1)) {
		//pop the non-table
		lua_pop(m_luaState, 1);
		return E_FAIL;
	} else {
		//When traversing a table, lua_next handles the iteration, but it needs nil to be the first popped key 
		lua_pushnil(m_luaState);  
		int tableIndex = -2;

		//Now the stack has the table at -2, nil at -1
		while (lua_next(m_luaState, tableIndex) != 0) {   //tableIndex is the index of the table on the stack
			/* `key' is at index -2 and `value' at index -1 */

			dprintf("%f - %f\n",
				(float)lua_tonumber(m_luaState, -2), (float)lua_tonumber(m_luaState, -1));
			//std::cout << lua_typename(m_luaState, lua_type(m_luaState, -2)) << " -- " << lua_typename(m_luaState, lua_type(m_luaState, -1)) << std::endl;
			lua_pop(m_luaState, 1);  /* removes `value'; keeps `key' for next iteration */
		}
	}

	//pop the table off the stack
	lua_pop(m_luaState, 1);
	return S_OK;
}

void LuaWrapper::PrintTable(const char* tableName)
{
	lua_getglobal(m_luaState, tableName);
	PrintTable(m_luaState);
}
void LuaWrapper::PrintTable(lua_State *L) 
{
    lua_pushnil(L);

    while(lua_next(L, -2) != 0)
    {
        if(lua_isstring(L, -1)) 
		{
			dprintf("%s = %s\n", lua_tostring(L, -2), lua_tostring(L, -1));
		}
		else if(lua_isnumber(L, -1))
		{ 
			dprintf("%s = %d\n", lua_tostring(L, -2), lua_tonumber(L, -1));
		} 
		else if(lua_istable(L, -1))
		{
			PrintTable(L);
		}

        lua_pop(L, 1);
    }
}

//GetFloatValueFromTable
// outputs S_OK on success, E_FAIL otherwise
// on S_OK, <result> holds the float value specified in the table named "tableName"
// named value "value". Put extra parameters on the end to deal with multiply nested tables.
// For example:

//  In Lua:
//     table = { value1 = 1, value2 = 2, subtable = { value1 = 3 } }
//  the call GetFloatValueFromTable(result, "table", "subtable", "value1")
// sets 'result' to 3.0f.
HRESULT LuaWrapper::GetFloatValueFromTable(float &result, const char* tableName, const char *value, ...)
{
	lua_getglobal(m_luaState, tableName);

	lua_pushstring(m_luaState, value);
	lua_gettable(m_luaState, -2);

	//Top of stack is now the value at tableName:value -- this could be another table
	// -2 is still tableName

	std::string nextValue;

	va_list ap;  //argument pointer
	va_start(ap, value);		//start after "value"

	int stackCount = 2;
	while(lua_istable(m_luaState, -1)) //while our first value is a table
	{
		nextValue = va_arg(ap, char*);
		lua_pushstring(m_luaState, nextValue.c_str());
		lua_gettable(m_luaState, -2);		//get the next value
		++stackCount;
	}

	va_end(ap); //cleanup any variable pointer stuff

	if(lua_isnumber(m_luaState, -1))
	{
		result = static_cast<float>(lua_tonumber(m_luaState, -1));
		lua_pop(m_luaState, stackCount);
		return S_OK;
	}
	else 
	{
		assert(false && "Tried to get a value from non-existant table, or tried to get non-number value as a float");
		lua_pop(m_luaState, stackCount);
		return E_FAIL;
	}
}

//GetStringValueFromTable
// Behaves the same as GetFloatValueFromTable. See the note on that function above.
HRESULT LuaWrapper::GetStringValueFromTable(std::string &result, const char* tableName, const char *value, ...)
{
	lua_getglobal(m_luaState, tableName);

	lua_pushstring(m_luaState, value);
	lua_gettable(m_luaState, -2);

	//Top of stack is now the value at tableName:value -- this could be another table
	// -2 is still tableName

	std::string nextValue;

	va_list ap;  //argument pointer
	va_start(ap, value);		//start after "value"

	int stackCount = 2;
	while(lua_istable(m_luaState, -1)) //while our first value is a table
	{
		nextValue = va_arg(ap, char*);
		lua_pushstring(m_luaState, nextValue.c_str());
		lua_gettable(m_luaState, -2);		//get the next value
		++stackCount;
	}

	va_end(ap); //cleanup any variable pointer stuff

	if(lua_isstring(m_luaState, -1))
	{
		result = lua_tostring(m_luaState, -1);
		lua_pop(m_luaState, stackCount);
		return S_OK;
	}
	else 
	{
		assert(false && "Tried to get a value from non-existant table, or tried to get number value as a string");
		lua_pop(m_luaState, stackCount);
		return E_FAIL;
	}
}

void LuaWrapper::Call(const char *function)
{
	lua_getglobal(m_luaState, function);
	lua_pcall(m_luaState, 0, 0, 0);
}
void LuaWrapper::Call(const char *function, void *argument)
{
	lua_getglobal(m_luaState, function);
	lua_pushlightuserdata(m_luaState, argument);
	if(0 != lua_pcall(m_luaState, 1, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}
void LuaWrapper::Call(const char *function, void *argument1, int argument2)
{
	lua_getglobal(m_luaState, function);
	lua_pushlightuserdata(m_luaState, argument1);
	lua_pushnumber(m_luaState, argument2);	
	if(0 != lua_pcall(m_luaState, 2, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}

void LuaWrapper::Call(const char *function, const char *argument) {
	lua_getglobal(m_luaState, function);
	lua_pushstring(m_luaState, argument);
	if(0 != lua_pcall(m_luaState, 1, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}

}

void LuaWrapper::Call(const char *function, int argument)
{
	lua_getglobal(m_luaState, function);
	lua_pushnumber(m_luaState, argument);
	if(0 != lua_pcall(m_luaState, 1, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}
void LuaWrapper::Call(const char *function, bool argument) 
{
	lua_getglobal(m_luaState, function);
	lua_pushboolean(m_luaState, argument);
	if(0 != lua_pcall(m_luaState, 1, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}
void LuaWrapper::Call(const char *function, float argument1, float argument2)
{
	lua_getglobal(m_luaState, function);
	lua_pushnumber(m_luaState, argument1);
	lua_pushnumber(m_luaState, argument2);
	if(0 != lua_pcall(m_luaState, 2, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}


void LuaWrapper::Call(const char *function, float argument)
{
	lua_getglobal(m_luaState, function);
	lua_pushnumber(m_luaState, argument);
	if(0 != lua_pcall(m_luaState, 1, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}

void LuaWrapper::Call(const char *function, float arg1, float arg2, float arg3)
{
	lua_getglobal(m_luaState, function);
	lua_pushnumber(m_luaState, arg1);
	lua_pushnumber(m_luaState, arg2);
	lua_pushnumber(m_luaState, arg3);
	
	if(0 != lua_pcall(m_luaState, 3, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}

void LuaWrapper::Call(const char *function, const char* arg1, float arg2, float arg3, float arg4)
{
	lua_getglobal(m_luaState, function);
	lua_pushstring(m_luaState, arg1);
	lua_pushnumber(m_luaState, arg2);
	lua_pushnumber(m_luaState, arg3);
	lua_pushnumber(m_luaState, arg4);
	
	if(0 != lua_pcall(m_luaState, 4, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}

void LuaWrapper::Call(const char *function, const char* arg1, float arg2, float arg3, float arg4, int arg5)
{
	lua_getglobal(m_luaState, function);
	lua_pushstring(m_luaState, arg1);
	lua_pushnumber(m_luaState, arg2);
	lua_pushnumber(m_luaState, arg3);
	lua_pushnumber(m_luaState, arg4);
	lua_pushnumber(m_luaState, arg5);
	
	if(0 != lua_pcall(m_luaState, 5, 0, 0))
	{
		dprintf("Error calling lua function %s: %s\n", function, lua_tostring(m_luaState, -1));
	}
}


void LuaWrapper::Register(std::string &functionName, int (*functionPointer)(lua_State*))
{
	lua_register(m_luaState, functionName.c_str(), (lua_CFunction)functionPointer);
}
