# LuaManagerEx

A single-file-header for embedding lua, offering helper functions and classes for metatable/userdata creation.

###### NOTE: In order for the example project to compile without errors, building the static libs (win32) of Lua is required.

## Userdata 

### .h
```cpp

class Entity;

DECLARE_LUA_OBJ(Entity, "Entity") // makes Lua_Entity wrapper

namespace LuaEntity
{
    int lua_destructor(lua_State* L);

    int lua_getname(lua_State* L);
    int lua_gethealth(lua_State* L);
  
    BEGIN_LUA_REG 
        DEFINE_LUA_GCREG(lua_destructor) // garbage collection
        DEFINE_LUA_REG("GetName", lua_getname)
        DEFINE_LUA_REG("GetHealth", lua_gethealth)
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
```
### .cpp
```cpp
int LuaEntity::lua_destructor(lua_State* L)
{
    Lua_Entity* obj = lua_checkudata<Lua_Entity>(L, 1, "Entity");

    delete obj;

    return 0;
}

int LuaEntity::lua_getname(lua_State* L)
{
    Lua_Entity* obj = lua_checkudata<Lua_Entity>(L, 1, "Entity");

    lua_pushstring(L, obj->GetName())

    return 1;
}

int LuaEntity::lua_gethealth(lua_State* L)
{
    Lua_Entity* obj = lua_checkudata<Lua_Entity>(L, 1, "Entity");

    luaL_pushinteger(L, obj->GetHealth())

    return 1;
}
```

## Userdata and Inheritance
With Lua, there is no official implementation of inheritance, meaning metatable/userdata cannot derive from a base class. As a result, [normally] the developer is required to implement the same function in each metatable. This header counters this.

### Old Way

```cpp
namespace LuaEnt
{
    static int lua_gethealth(lua_State* L)
    {
        Lua_Entity* ptr = lua_checkudata<Lua_Entity>(L, 1, "Entity");

        lua_pushinteger(L, ptr->GetHealth());

        return 1;
    }
    static int lua_sethealth(lua_State* L)
    {
        Lua_Entity* ptr = lua_checkudata<Lua_Entity>(L, 1, "Entity");
        int iHealth = luaL_checkint(L, 2);

        ptr->SetHealth(iHealth);

        return 1;
    }
    
    BEGIN_LUA_REG 
        DEFINE_LUA_REG("GetHealth", lua_gethealth)
        DEFINE_LUA_REG("SetHealth", lua_sethealth)
    END_LUA_REG
}

namespace LuaPlayer
{
    // functiom from base class 'Entity'
    static int lua_gethealth(lua_State* L)
    {
        Lua_Player* ptr = lua_checkudata<Lua_Player>(L, 1, "Player");

        lua_pushinteger(L, ptr->GetHealth());

        return 1;
    }
    
    // functiom from base class 'Entity'
    static int lua_sethealth(lua_State* L)
    {
        Lua_Player* ptr = lua_checkudata<Lua_Player>(L, 1, "Player");
        int iHealth = luaL_checkint(L, 2);

        ptr->SetHealth(iHealth);

        return 1;
    }
    
    // <insert other member functions here>...
    BEGIN_LUA_REG 
        DEFINE_LUA_REG("GetHealth", lua_gethealth)
        DEFINE_LUA_REG("SetHealth", lua_sethealth)
    END_LUA_REG
}

namespace LuaNPC
{
    // functiom from base class 'Entity'
    static int lua_gethealth(lua_State* L)
    {
        Lua_NPC* ptr = lua_checkudata<Lua_NPC>(L, 1, "NPC");

        lua_pushinteger(L, ptr->GetHealth());

        return 1;
    }
    
    // functiom from base class 'Entity'
    static int lua_sethealth(lua_State* L)
    {
        Lua_NPC* ptr = lua_checkudata<Lua_NPC>(L, 1, "NPC");
        int iHealth = luaL_checkint(L, 2);

        ptr->SetHealth(iHealth);

        return 1;
    }

    // <insert other member functions here>...
    BEGIN_LUA_REG 
        DEFINE_LUA_REG("GetHealth", lua_gethealth)
        DEFINE_LUA_REG("SetHealth", lua_sethealth)
    END_LUA_REG
}
```

### New Way
```cpp
DECLARE_LUA_OBJ(NPC, "NPC") // wrapper for npc - Lua_NPC
DECLARE_LUA_OBJ(Player, "Player") // wrapper for player - Lua_Player

LuaTypes g_EntityTypes = { 
    LuaTypeEntry("NPC", LUA_HANDLER_NAME(NPC)),
    LuaTypeEntry("Player", LUA_HANDLER_NAME(Player))
};

namespace LuaEnt
{
    static int lua_gethealth(lua_State* L)
    {
        Entity* ptr = lua_checkudata_poly<Entity>(L, 1, g_EntityTypes);

        lua_pushinteger(L, ptr->GetHealth());

        return 1;
    }
    
    static int lua_sethealth(lua_State* L)
    {
        Entity* ptr = lua_checkudata_poly<Entity>(L, 1, g_EntityTypes);
        int iHealth = luaL_checkint(L, 2);

        ptr->SetHealth(iHealth);

        return 1;
    }
}

namespace LuaPlayer
{
    
    // <insert other member functions here>...
    
    BEGIN_LUA_REG 
        DEFINE_LUA_REG("GetHealth", LuaEnt::lua_gethealth)
        DEFINE_LUA_REG("SetHealth", LuaEnt::lua_sethealth)
    END_LUA_REG
}

namespace LuaNPC
{
    // <insert other member functions here>...
    
    BEGIN_LUA_REG 
        DEFINE_LUA_REG("GetHealth", LuaEnt::lua_gethealth)
        DEFINE_LUA_REG("SetHealth", LuaEnt::lua_sethealth)
    END_LUA_REG
}
```

## Further Examples

### Pushing userdata

```cpp
MyClass* ptr = new MyClass(); 

lua_pushobject(L, new Lua_MyClass(ptr), "MyMetatableName");
```
