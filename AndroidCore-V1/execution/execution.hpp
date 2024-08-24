#pragma once

#include <queue>
#include <Luau/Compiler.h>
#include <Luau/BytecodeBuilder.h>
#include <Luau/BytecodeUtils.h>
#include <mutex>
#include <functional>


namespace exploit
{
    class bytecode_encoder_t : public Luau::BytecodeEncoder {
        inline void encode(uint32_t* data, size_t count) override
        {
            for (auto i = 0u; i < count;)
            {
                auto& opcode = *reinterpret_cast<uint8_t*>(data + i);
                i += Luau::getOpLength(LuauOpcode(opcode));
                opcode *= 227;
            }
        }
    };

    class execute {
    public:
        static auto get_singleton() -> execute* {
            static execute* dizz = nullptr;

            if (dizz == nullptr)
                dizz = new execute();

            return dizz;
        }
        auto execute_script(const std::string& script) -> void;
    };
}