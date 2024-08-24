/*
#include <lua.h>
#include <lualib.h>
#include <lmem.h>
#include <lgc.h>
*/

static int getgenv(lua_State* ls) // register the luau first to register the lua state here
{
        lua_pushvalue(ls, LUA_GLOBALSINDEX);
	return 1;
}

static int getreg(lua_State* ls) // register the luau first to register the lua state here
{
	lua_pushvalue(ls, LUA_REGISTRYINDEX);
	return 1;
}

static int gettenv(lua_State* ls) // register the luau first to register the lua state here
{
	luaL_checktype(ls, 1, LUA_TTHREAD);

    auto thread = lua_tothread(ls, 1);

    lua_pushvalue(thread, LUA_GLOBALSINDEX);
    lua_xmove(thread, ls, 1);
	return 1;
}

static int getthread(lua_State* ls) // register the luau first to register the lua state here
{
	lua_pushthread(ls);
	return 1;
}

static int identifyexecutor(lua_State* ls)
{
   // will return AndroidCore 1.0.0
    lua_pushstring(ls, exploit_configuration::exploit_name.c_str( ));
    lua_pushstring(ls, exploit_configuration::exploit_version( ));
    return 2; 
}

static int isLuau(lua_State* ls)
{
    lua_pushboolean(ls, true); 
    return 1;
}

static const luaL_Reg function[] = { // Should be getting an error because of unregistered luau.
	{"getgenv", getgenv},
	{"getreg", getreg},
	{"gettenv", gettenv},
	{"getthread", getthread},
        {"identifyexecutor", identifyexecutor},
        {"isLuau", isLuau},
}
