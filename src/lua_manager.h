#pragma once

#include "lua.hpp"

#include <vector>
#include <tuple>


#define DECLARE_LUA_OBJ(_class, name) \
	class Lua_##_class : public ILuaObject<_class> { \
		using ILuaObject<_class>::ILuaObject; \
	public: \
		const char* GetName() override\
		{\
			return #name;\
		}\
	};

#define MAKE_LUA_HANDLER(type)\
	static void* Lua_GetUData_ ## type(lua_State* L, int n, const char* mtname, void* ptr) { Lua_ ## type *p = *(Lua_ ## type**)ptr; return p->GetInstance(); }

#define BEGIN_LUA_REG const luaL_Reg regs[] = {



#define END_LUA_REG {NULL, NULL} };

#define DEFINE_LUA_REG(name, func) \
	{name, func},

#define DEFINE_LUA_GCREG(func) \
	{"__gc", func},

#define LUA_HANDLER_NAME(type) Lua_GetUData_ ## type


typedef void*(*LuaHandler)(lua_State* L, int n, const char*, void* udata); 

typedef std::tuple<const char*, LuaHandler> LuaTypeEntry;
typedef std::vector<LuaTypeEntry> LuaTypes;

#ifndef abstract_class
	#define abstract_class class __declspec( novtable )
#endif

abstract_class ILuaHandle
{
public:
	inline lua_State* GetState() { return m_lState; }
	inline const char* GetID() { return _strdup(m_szID); }

	ILuaHandle(const char* szID)
	{
		m_szID = szID;

	//	Setup();
	}

	bool IsValid()
	{
		return m_lState != nullptr;
	}

	bool IsLoaded()
	{
		return m_bLoaded;
	}

	virtual void PreSetup() {}
	virtual void PostSetup() {}

	virtual void RegisterFunctions(lua_State* L) = 0;
	virtual void RegisterMetatables(lua_State* L) = 0;
	virtual void RegisterLibs(lua_State* L) = 0;

	void Setup()
	{
		PreSetup();

		m_lState = luaL_newstate();
		luaL_openlibs(m_lState);

		RegisterLibs(m_lState);
		RegisterFunctions(m_lState);
		RegisterMetatables(m_lState);

		PostSetup();

		m_bLoaded = true;
	}


private:
	lua_State* m_lState = nullptr;
	bool m_bLoaded = false;

protected:
	const char* m_szID = "";

	virtual ~ILuaHandle()
	{
		lua_close(m_lState);

		m_bLoaded = false;
	}
};


// Source support! NOTE: "shared" functions belong in each class!
#ifdef _SERVER
class CLuaStateHandle : public ILuaStateHandle 
{
	void RegisterFunctions() override
	{

	}

	void RegisterMetatables() override
	{

	}

	void RegisterLibs() override
	{

	}
};

extern CServerStateHandle* g_LuaHandle; // define somewhere in a source file
#endif

#ifdef CLIENT_DLL
class C_LuaStateHandle : public ILuaStateHandle
{
	void RegisterFunctions() override
	{

	}

	void RegisterMetatables() override
	{

	}

	void RegisterLibs() override
	{

	}
};

extern CClientStateHandle* g_LuaHandle; // define somewhere in a source file
#endif

template<typename T>
abstract_class ILuaObject
{
public:
	virtual const char* GetName() = 0;

	ILuaObject() {}

	ILuaObject(T* pInstance) { m_pInstance = pInstance;}

	inline T* GetInstance() { return m_pInstance;  }
	inline void SetInstance(T* pInstance) { m_pInstance = pInstance; }

	inline bool IsValid() { return m_pInstance != nullptr; }
private:
	T* m_pInstance = nullptr;
};


/**
	* Iterates a LuaTypes vector, searching for an entry with the correct userdata type at stack pos n.
	*
	* @param L - The lua state
	* @param n - The index on the stack
	* @param types - a LuaTypes vector.
	* @returns An address or NULL if a correct entry in the provided LuaTypes vector was not found.
	* 
	* @example ddd
*/
static void* lua_checkudata_poly(lua_State* L, int n, LuaTypes& types)
{
	for (auto it = begin(types); it != end(types); ++it) {
		const char* szMetaName = std::get<0>(*it);
		
		void* udata = luaL_testudata(L, n, szMetaName);

		if (udata != NULL)
			return std::get<1>(*it)(L, n, szMetaName, udata);
	}

	return NULL;
}

/**
	* Iterates a LuaTypes vector, searching for an entry with the correct userdata type at stack pos n.
	*
	* @param T the base class
	* @param L - The lua state
	* @param n - The index on the stack
	* @param types - a LuaTypes vector.
	* @returns A casted pointer or NULL if a correct entry in the provided LuaTypes vector was not found.
*/
template <class T>
static T* lua_checkudata_poly(lua_State* L, int n, LuaTypes& types)
{
	for (auto it = begin(types); it != end(types); ++it) {
		const char* szMetaName = std::get<0>(*it);

		void* udata = luaL_testudata(L, n, szMetaName);

		if (udata != NULL)
			return static_cast<T*>(std::get<1>(*it)(L, n, szMetaName, udata));
	}

	return NULL;
}


/**
	* Pushes userdata to the stack. 
	*
	* @param T the type of data
	* @param L - The lua state.
	* @param pObj - Pointer to the desired object.
	* @param metatable - Name of the correlated metatable. 
*/
template<typename T>
static void lua_pushobject(lua_State* L, T* pObj, const char* metatable)
{
	T** ud = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
	*ud = pObj;

	luaL_getmetatable(L, metatable);
	lua_setmetatable(L, -2);
}


/**
	* Receives userdata from the stack.
	*
	* @param T the type of data
	* @param L - The lua state.
	* @param mtname - Name of the correlated metatable.
	* @returns Pointer to the retrieved object. 
*/
template<typename T>
static T* lua_checkudata(lua_State* L, int n, const char* mtname)
{
	return *(T**)luaL_checkudata(L, n, mtname);
}