#pragma once

#include <lua.h>
#include <lualib.h>
#include <lgc.h>

namespace exploit::environment{
    auto blank_function(lua_State * ls) -> int;

    auto registry_init(lua_State* ls) -> void;
    auto register_lib(lua_State* ls, const char* libname, const luaL_Reg* lib) -> void;

    auto functions(lua_State* ls) -> void;
    auto cclosure(lua_State* ls) -> void;
}