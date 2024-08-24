#pragma once

#include <unordered_map>
#include <lobject.h>
#include <lua.h>

namespace checkers
{
    extern std::unordered_map<Closure*, lua_CFunction> cclosure_map;
    extern std::unordered_map<Closure*, Closure*> newcclosure_map;

    auto call_handler(lua_State* ls) -> int;
    auto pushcclosure(lua_State* ls, lua_CFunction fn, const char* debugname, int nups) -> void;
}