#include <thread>
#include "execution.hpp"

std::mutex queue_mutex;

auto exploit::execute::execute_script(const std::string& script) -> void
{
    if (script.empty()) return;

    static auto encoder = bytecode_encoder_t();
    auto bytecode = Luau::compile(script, { }, { }, &encoder);

    lua_call(ls, 1, 0);
    lua_pop(ls, 1);
}