#ifndef LUAWRAPPER_H
#define LUAWRAPPER_H

#ifdef _MSC_VER
#pragma once
#endif

#include <string>

//lua includes
extern "C" {
	#include "Lua 5.1/src/lua.h"
	#include "Lua 5.1/src/lualib.h"
	#include "Lua 5.1/src/lauxlib.h"
}


#include <windows.h>


namespace Lua {

	class LuaWrapper {
	public:
		lua_State *GetLuaState();
		static LuaWrapper* Instance();
		void Initialize();

		HRESULT TraverseTable(const char* tableName);
		void PrintTable(const char* tableName);
		void PrintTable(lua_State *L);

		HRESULT GetStringValueFromTable(std::string &result, const char *tableName, const char *value, ...);
		HRESULT GetFloatValueFromTable(float &result, const char *tableName, const char *value, ...);

		void Call(const char *function);
		void Call(const char *function, int argument);
		void Call(const char *function, bool argument);
		void Call(const char *function, float argument);
		void Call(const char *function, const char *argument);
		void Call(const char *function, float argument1, float argument2);
		void Call(const char *function, void *argument);
		void Call(const char *function, void *argument1, int argument2);
		void Call(const char *function, float arg1, float arg2, float arg3);
		void Call(const char *function, const char* arg1, float arg2, float arg3, float arg4);
		void Call(const char *function, const char* arg1, float arg2, float arg3, float arg4, int arg5);

		static int MakeVector2(lua_State *L);
		static int MakeVector3(lua_State *L);
		static int MakeVector4(lua_State *L);

		void Register(std::string &functionName, int (*functionPointer)(lua_State*));

	private:
		lua_State  *m_luaState;

		LuaWrapper();
		~LuaWrapper();

	};
}

#endif //LUAWRAPPER_H