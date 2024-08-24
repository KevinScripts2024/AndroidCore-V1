// I know checkers doesn't exist on Android. just wanted to make it the "nop" way.

#include "checkers.hpp"

#include <lstate.h>
#include <lapi.h>

namespace checkers
{
    std::unordered_map<Closure*, lua_CFunction> cclosure_map = { };
    std::unordered_map<Closure*, Closure*> newcclosure_map = { };

    auto call_handler(lua_State* ls) -> int
    {
        auto idx = cclosure_map.find(curr_func(ls));

        if (idx != cclosure_map.end())
        {
            return idx->second(ls);
        }
        return 0;
    }

    auto pushcclosure(lua_State* ls, lua_CFunction fn, const char* debugname, int nups) -> void
    {
        lua_pushcclosurek(ls, call_handler, debugname, nups, 0);
        cclosure_map[clvalue(luaA_toobject(ls, -1))] = fn;

        LOGD(" cc_pushcclosure | %p -> %s - %p - %i ", clvalue(luaA_toobject(ls, -1)), debugname, fn, nups);
    }
}