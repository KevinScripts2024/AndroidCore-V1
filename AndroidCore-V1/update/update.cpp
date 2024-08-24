#include "update.hpp"
#include "../utility/utility.hpp"

lua_State* (*funcs::getmainstate)(lua_State* rL) = nullptr;
int (*funcs::scriptcontextresume)(int unk, std::uintptr_t sc, std::uintptr_t* ref, int nargs, int, int) = nullptr;

auto api::addresses::eLEnc(std::uintptr_t sc)->std::uintptr_t
{
	return *(std::uintptr_t*)(sc + 378) + (sc + 378); // maybe outdated who knows
}

void funcs::init() {
	scriptcontextresume = reinterpret_cast<decltype(scriptcontextresume)>(utility::memory::rebase("libroblox.so", api::addresses::scriptcontextresume));
	getmainstate = reinterpret_cast<decltype(getmainstate)>(utility::memory::rebase("libroblox.so", api::addresses::getmainstate));
}

auto get_context_level_permissions(int identity) -> std::uintptr_t
{
    unsigned int v1;
    int result;

    v1 = identity - 1;
    result = 0;
    if (v1 <= 8)
    {
        switch (v1)
        {
        case 0u:
        case 3u:
            result = 3;
            break;
        case 1u:
            return result;
        case 2u:
        case 5u:
            result = 11;
            break;
        case 4u:
            result = 1;
            break;
        case 6u:
        case 7u:
            result = 63;
            break;
        case 8u:
            result = 12;
            break;
        }
    }

    return result;
}