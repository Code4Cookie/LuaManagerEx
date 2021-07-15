#include "lua_imp.h"
#include "internals.h"
#include <iostream>

int main()
{
	g_LuaHandle = new CGELuaHandle("MyLuaHandle");
	g_LuaHandle->Setup();

	luaL_dostring(g_LuaHandle->GetState(), R"(
		local ply = players.Spawn()

		ply:SetHealth(100)

		print(ply:GetHealth())
	)");
}