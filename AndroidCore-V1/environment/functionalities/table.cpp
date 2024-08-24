#include <lua.h>
#include <lualib.h>
#include <lapi.h>
#include <lstate.h>

static int getrawmetatable(lua_State* ls)
{
    if (!lua_getmetatable(ls, 1)) lua_pushnil(ls);
    return 1;
}

static int setrawmetatable(lua_State* ls)
{
    lua_settop(ls, 2);
    lua_setmetatable(ls, -2);
    return 1;
}

static int setreadonly(lua_State* ls)
{
    lua_setreadonly(ls, 1, luaL_checkboolean(ls, 2));
    return 0;
}

static int isreadonly(lua_State* ls)
{
    lua_pushboolean(ls, lua_getreadonly(ls, 1));
    return 1;
}

static int makewriteable(lua_State* ls)
{
    lua_setreadonly(ls, 1, 1);
    return 0;
}

static int makereadonly(lua_State* ls)
{
    lua_setreadonly(ls, 1, 0);
    return 0;
}

static int getnamecallmethod(lua_State* ls)
{
    lua_pushstring(ls, lua_namecallatom(ls, nullptr));
    return 1;
}

static int setnamecallmethod(lua_State* ls)
{
    luaL_checktype(ls, 1, LUA_TSTRING);

    ls->namecall = tsvalue(luaA_toobject(ls, 1));
    return 0;
}

static const luaL_Reg funcs[] = {
    {"getrawmetatable", getrawmetatable},
    {"setrawmetatable", setrawmetatable},
    {"setreadonly", setreadonly},
    {"isreadonly", isreadonly},
    {"make_writeable", makewriteable},
    {"make_readonly", makereadonly},
    {"getnamecallmethod", getnamecallmethod},
    {"setnamecallmethod", setnamecallmethod},

    {nullptr, nullptr}
};
