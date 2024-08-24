#include <lua.h>
#include <lualib.h>
#include "../../filesystem/filesystem.hpp"
#include "../../execution/execution.hpp"
#include "../../utility/utility.hpp"

std::string header_get_file_path(lua_State* ls, bool check_ext = true)
{
    size_t filenameCSize;
    auto filenameCstr = luaL_checklstring(ls, 1, &filenameCSize);

    auto filename = std::string(filenameCstr, filenameCSize);
    if (const auto ret_code = file_system::is_valid_path(filename); ret_code != file_system::Ok)
    {
        if (ret_code == file_system::Blacklisted_ext && check_ext)
        {
            luaL_argerror(ls, 1, "Path contains a blacklisted extension.");
            return "";
        }

        if (ret_code == file_system::Invalid_path)
        {
            luaL_argerror(ls, 1, "Path is invalid.");
            return "";
        }

        if (check_ext == true)
        {
            luaL_error(ls, "What the ####. This isn't supposed to happen.");
            return "";
        }
    }

    auto file_path = file_system::get_workspace_path() + "/" + filename;

    return file_path;
}

/*
static int readfile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    const auto [result_contents, ret_code] = file_system::read_file(file_path);
    if (ret_code != file_system::Ok)
    {
        if (ret_code == file_system::No_Exist)
        {
            luaL_argerror(ls, 1, "File or Path does not exist.");
            return 0;
        }

        if (ret_code == file_system::Cant_Open)
        {
            luaL_argerror(ls, 1, "Cant open file.");
            return 0;
        }
    }

    lua_pushlstring(ls, result_contents.c_str(), result_contents.size());
    return 1;
}
*/

static int writefile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    size_t contentsCSize;
    auto contentsCstr = luaL_checklstring(ls, 2, &contentsCSize);

    auto contents = std::string(contentsCstr, contentsCSize);

    file_system::create_directories(file_path);
    if (const auto ret_code = file_system::write_file(file_path, contents); ret_code != file_system::Ok)
    {
        if (ret_code == file_system::Cant_Open)
        {
            luaL_argerror(ls, 1, "Cant open file.");
            return 0;
        }
    }

    return 0;
}

static int appendfile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    size_t contentsCSize;
    auto contentsCstr = luaL_checklstring(ls, 2, &contentsCSize);

    auto contents = std::string(contentsCstr, contentsCSize);

    file_system::create_directories(file_path);
    if (const auto ret_code = file_system::append_file(file_path, contents); ret_code != file_system::Ok)
    {
        if (ret_code == file_system::Cant_Open)
        {
            luaL_argerror(ls, 1, "Cant open file.");
            return 0;
        }
    }

    return 0;
}

static int listfiles(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    auto file_list = file_system::list_files(file_path);

    int index = 0;
    lua_newtable(ls);
    for (const auto& file : file_list)
    {
        lua_pushlstring(ls, file.data(), file.size());
        lua_rawseti(ls, -2, ++index);
    }

    return 1;
}

static int isfolder(lua_State* ls)
{
    auto file_path = header_get_file_path(ls, false);

    lua_pushboolean(ls, file_system::is_directory(file_path));
    return 1;
}

static int isfile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls, false);

    lua_pushboolean(ls, file_system::is_regular_file(file_path));
    return 1;
}

static int makefolder(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);
    file_path += "/";

    file_system::create_directories(file_path);
    return 0;
}

static int delfolder(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    file_system::remove_directory(file_path);
    return 0;
}

static int delfile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    file_system::remove_file(file_path);
    return 0;
}

/*
static int loadfile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);
    std::string chunkname = lua_isstring(ls, 2) ? lua_tostring(ls, 2) : utility::randomStr(4);

    const auto [contents, ret_code] = file_system::read_file(file_path);
    if (ret_code != file_system::Ok)
    {
        if (ret_code == file_system::No_Exist)
        {
            luaL_argerror(ls, 1, "File or Path does not exist.");
            return 0;
        }

        if (ret_code == file_system::Cant_Open)
        {
            luaL_argerror(ls, 1, "Cant open file.");
            return 0;
        }
    }

    auto bytecode = Luau::compile(contents, { }, { });

    if (ls, chunkname.c_str(), bytecode.c_str(), bytecode.size(), 0)
    {
        lua_pushnil(ls);
        lua_pushstring(ls, lua_tostring(ls, -2));
        return 2;
    }

    return 1;
}
*/

/*
static int dofile(lua_State* ls)
{
    auto file_path = header_get_file_path(ls);

    const auto [contents, ret_code] = file_system::read_file(file_path);
    if (ret_code != file_system::Ok)
    {
        if (ret_code == file_system::No_Exist)
        {
            luaL_argerror(ls, 1, "File or Path does not exist.");
            return 0;
        }

        if (ret_code == file_system::Cant_Open)
        {
            luaL_argerror(ls, 1, "Cant open file.");
            return 0;
        }
    }

    exploit::execute::get_singleton()->execute_script(contents);
    return 0;
}
*/

static int getcustomasset(lua_State* ls)
{
    size_t filenameCSize;
    auto filenameCstr = luaL_checklstring(ls, 1, &filenameCSize);

    auto filename = std::string(filenameCstr, filenameCSize);
    if (const auto ret_code = file_system::is_valid_path(filename); ret_code != file_system::Ok)
    {
        if (ret_code == file_system::Blacklisted_ext)
        {
            luaL_argerror(ls, 1, "Path contains a blacklisted extension.");
            return 0;
        }

        if (ret_code == file_system::Invalid_path)
        {
            luaL_argerror(ls, 1, "Path is invalid.");
            return 0;
        }
    }

    auto file_path = file_system::get_workspace_path() + "/" + filename;
    auto file_asset_path = file_system::get_asset_path() + "/" + filename;
    file_system::create_directories(file_asset_path);

    if (file_system::copy_file(file_path, file_asset_path) != file_system::Ok)
    {
        lua_pushstring(ls, "-- Failed to get custom asset.");
        return 1;
    }

    auto rbxasset_path = std::string("rbxasset://" + exploit_configuration::exploit_name + "/");
    rbxasset_path += filename;

    lua_pushstring(ls, rbxasset_path.c_str());
    return 1;
}

static const luaL_Reg funcs[] = {
   // {"readfile", readfile},
    {"writefile", writefile},
    {"appendfile", appendfile},
    {"listfiles", listfiles},
    {"isfolder", isfolder},
    {"isfile", isfile},
    {"makefolder", makefolder},
    {"delfolder", delfolder},
    {"delfile", delfile},
   // {"loadfile", loadfile},
   // {"dofile", dofile},
    {"getcustomasset", getcustomasset},

    {nullptr, nullptr}
};