#include <lua.h>
#include "../../checkers/checkers.hpp"
#include <lualib.h>
#include <lstate.h>
#include <Luau/Compiler.h> 
#include <lobject.h>
#include <lapi.h>
#include <lfunc.h>
#include <lgc.h>

#include "../../update/update.hpp"
#include "../../utility/utility.hpp"
#include "../../execution/execution.hpp"
#include <unordered_set>

static int newcclosure_handler(lua_State* ls)
{
    const auto nargs = lua_gettop(ls);

    void* LClosure = reinterpret_cast<void*>(checkers::newcclosure_map.find(clvalue(ls->ci->func))->second);
    if (LClosure == nullptr)
    {
        return 0;
    }

    luaC_threadbarrier(ls);
    setclvalue(ls, ls->top, LClosure);
    ls->top++;

    lua_insert(ls, 1);

    const auto res = lua_pcall(ls, nargs, LUA_MULTRET, 0);
    if (res && res != LUA_YIELD && std::strcmp(lua_tostring(ls, -1), "attempt to yield across metamethod/C-call boundary") == 0)
    {
        return lua_yield(ls, 0);
    }

    return lua_gettop(ls);
}

static int newcclosure(lua_State* ls)
{
    LOGD(" LuauEnvCall -> newcclosure - CallingThread -> %p", ls);

    luaL_checktype(ls, 1, LUA_TFUNCTION);

    if (lua_iscfunction(ls, 1)) { return 1; }

    lua_ref(ls, 1);
    lua_pushcclosure(ls, newcclosure_handler, nullptr, 0);
    checkers::newcclosure_map[clvalue(luaA_toobject(ls, -1))] = clvalue(luaA_toobject(ls, 1)); // link the lclosure to the handler

    return 1;
}

std::unordered_set<std::string> chunknames;
static int loadstring(lua_State* ls)
{
    auto str = luaL_checkstring(ls, 1);
    std::string chunkname = lua_isstring(ls, 2) ? lua_tostring(ls, 2) : utility::randomStr(4);

    auto bytecode = Luau::compile(str, { }, { });

    if (ls, chunkname.c_str(), bytecode.c_str(), bytecode.size(), 0)
    {
        lua_pushnil(ls);
        lua_pushstring(ls, lua_tostring(ls, -2));
        return 2;
    }

    chunknames.insert(chunkname);
    return 1;
}

static const luaL_Reg funcs[] = {
    {"newcclosure", newcclosure},
    {"loadstring", loadstring},

    {nullptr, nullptr}
};

auto exploit::environment::cclosure(lua_State* ls) -> void
{
    lua_pushvalue(ls, LUA_GLOBALSINDEX);
    register_lib(ls, nullptr, funcs);
    lua_pop(ls, 1);
}