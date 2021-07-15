#include "lua_imp.h"
#include "internals.h"

#include <iostream>

CGELuaHandle* g_LuaHandle = NULL;

LuaTypes g_EntityTypes = { 
	LuaTypeEntry("NPC", LUA_HANDLER_NAME(NPC)),
	LuaTypeEntry("Player", LUA_HANDLER_NAME(Player))
};

/*************************************
	_______   ____________________  __
   / ____/ | / /_  __/  _/_  __/\ \/ /
  / __/ /  |/ / / /  / /  / /    \  /
 / /___/ /|  / / / _/ /  / /     / /
/_____/_/ |_/ /_/ /___/ /_/     /_/
*************************************/

int LuaEntity::lua_getname(lua_State* L)
{
	Entity* ptr = lua_checkudata_poly<Entity>(L, 1, g_EntityTypes);

	lua_pushstring(L, ptr->GetName());

	return 1;
}

int LuaEntity::lua_gethealth(lua_State* L)
{
	Entity* ptr = lua_checkudata_poly<Entity>(L, 1, g_EntityTypes);

	lua_pushinteger(L, ptr->GetHealth());

	return 1;
}

int LuaEntity::lua_setmodel(lua_State* L)
{
	Entity* ptr = lua_checkudata_poly<Entity>(L, 1, g_EntityTypes);
	const char* szModel = luaL_checkstring(L, 2);

	ptr->SetModel(szModel);

	return 1;
}

int LuaEntity::lua_sethealth(lua_State* L)
{
	Entity* ptr = lua_checkudata_poly<Entity>(L, 1, g_EntityTypes);
	int iHealth = luaL_checkint(L, 2);

	ptr->SetHealth(iHealth);

	return 1;
}

/************************************
	____  __    _____  ____________
   / __ \/ /   /   \ \/ / ____/ __ \
  / /_/ / /   / /| |\  / __/ / /_/ /
 / ____/ /___/ ___ |/ / /___/ _, _/
/_/   /_____/_/  |_/_/_____/_/ |_|

************************************/

int LuaPlayer::lua_destructor(lua_State* L)
{
	Lua_Player* obj = lua_checkudata<Lua_Player>(L, 1, "Player");

	delete obj;

	return 0;
}

int LuaPlayer::lua_getnick(lua_State* L)
{
	Lua_Player* obj = lua_checkudata<Lua_Player>(L, 1, "Player");

	lua_pushstring(L, obj->GetInstance()->GetNick());

	return 1;

}

int LuaPlayer::lua_jump(lua_State* L)
{
	Lua_Player* obj = lua_checkudata<Lua_Player>(L, 1, "Player");

	obj->GetInstance()->Jump();

	return 1;
}

int LuaPlayer::lua_shoot(lua_State* L)
{
	Lua_Player* obj = lua_checkudata<Lua_Player>(L, 1, "Player");

	obj->GetInstance()->Shoot();

	return 1;
}

/*********************
	_   ______  ______
   / | / / __ \/ ____/
  /  |/ / /_/ / /
 / /|  / ____/ /___
/_/ |_/_/    \____/

*********************/

int LuaNPC::lua_destructor(lua_State* L)
{
	Lua_NPC* obj = lua_checkudata<Lua_NPC>(L, 1, "NPC");

	delete obj;

	return 0;
}

int LuaNPC::lua_setaggro(lua_State* L)
{
	Lua_NPC* obj = lua_checkudata<Lua_NPC>(L, 1, "NPC");
	int iAggro = luaL_checkint(L, 2);

	obj->GetInstance()->SetAggression(iAggro);

	return 1;
}



int LuaPlayerLib::lua_spawn(lua_State* L)
{
	Player* pPlayer = new Player("todd howard"); // 16 times the detail

	lua_pushobject(L, new Lua_Player(pPlayer), "Player");

	return 1;
}