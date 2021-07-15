/************ LUA IMPLIMENTATION FILE ************/

#pragma once

#include "lua_manager.h"

class Entity;
class NPC;
class Player;


// DECLARE_LUA_OBJ(Entity, "Entity")
DECLARE_LUA_OBJ(NPC, "NPC")
DECLARE_LUA_OBJ(Player, "Player")

// MAKE_LUA_HANDLER(Entity);
MAKE_LUA_HANDLER(NPC);
MAKE_LUA_HANDLER(Player);

extern LuaTypes g_EntityTypes;


// both Player and NPC classes inherit functions from the Entity base class.
namespace LuaEntity
{
	int lua_getname(lua_State* L);
	int lua_gethealth(lua_State* L);

	int lua_setmodel(lua_State* L);
	int lua_sethealth(lua_State* L);
}

namespace LuaPlayer
{
	int lua_destructor(lua_State* L);
	int lua_getnick(lua_State* L);

	int lua_jump(lua_State* L);
	int lua_shoot(lua_State* L);

	BEGIN_LUA_REG 
		DEFINE_LUA_GCREG(lua_destructor) // garbage collection
		DEFINE_LUA_REG("GetNick", lua_getnick)
		DEFINE_LUA_REG("GetHealth", LuaEntity::lua_gethealth)
		DEFINE_LUA_REG("SetHealth", LuaEntity::lua_sethealth)
		DEFINE_LUA_REG("Jump", lua_jump)
		DEFINE_LUA_REG("Shoot", lua_shoot)
	END_LUA_REG

	static int registerclass(lua_State* L)
	{
		luaL_newmetatable(L, "Player");

		luaL_setfuncs(L, regs, 0);

		lua_pushvalue(L, -1);

		lua_setfield(L, -1, "__index");

		lua_pop(L, 1);

		return 0;
	}

}

namespace LuaNPC
{
	int lua_destructor(lua_State* L);
	int lua_setaggro(lua_State* L);

	BEGIN_LUA_REG 
		DEFINE_LUA_GCREG(lua_destructor) // garbage collection
		DEFINE_LUA_REG("SetAggression", lua_setaggro)
	END_LUA_REG

	static int registerclass(lua_State* L)
	{
		luaL_newmetatable(L, "NPC");

		luaL_setfuncs(L, regs, 0);

		lua_pushvalue(L, -1);

		lua_setfield(L, -1, "__index");

		lua_pop(L, 1);

		return 0;
	}
}

namespace LuaPlayerLib
{
	int lua_spawn(lua_State* L);
	
	BEGIN_LUA_REG
		DEFINE_LUA_REG("Spawn", lua_spawn)
	END_LUA_REG
	
	static int registerlib(lua_State* l)
	{
		luaL_newlib(l, regs);
		lua_setglobal(l, "players");

		return 0;
	}
}



class CGELuaHandle : public ILuaHandle
{
public:
	using ILuaHandle::ILuaHandle;

	void RegisterFunctions(lua_State* L) override
	{

	}



	void RegisterMetatables(lua_State* L) override
	{
		LuaPlayer::registerclass(L);
		LuaNPC::registerclass(L);
	}

	void RegisterLibs(lua_State* L) override
	{
		LuaPlayerLib::registerlib(L);
	}
};

extern CGELuaHandle* g_LuaHandle; // TODO: Define in a source file!