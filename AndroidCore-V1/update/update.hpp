#pragma once

#include <lstate.h>
#include <cstdint>
#include <string>


// Roblox v2.639.688
namespace api
{
	namespace addresses {
			auto eLEnc(std::uintptr_t sc)->std::uintptr_t;

			constexpr std::uintptr_t tasksched = 0x4064A72; // unused

			constexpr std::uintptr_t getmainstate = 0x24B622C; // "Unable to create a new thread for %s"
			constexpr std::uintptr_t scriptcontextresume = 0x18E5572; // "$Script"
	}


    // Nop's code we love you
    struct live_thread_ref
    {
        int unk_0;
        lua_State* th;
        int thread_id;
    };

    struct weak_thread_ref_t
    {
        std::uint8_t pad_0[8];

        weak_thread_ref_t* previous;
        weak_thread_ref_t* next;
        live_thread_ref* livethreadref;
    };

    struct ExtraSpace_t
    {
        struct Shared {
            int threadCount;
            std::uintptr_t script_context;
            ExtraSpace_t* allThreads;
        };

        ExtraSpace_t* previous;
        size_t count;
        ExtraSpace_t* next;
        std::shared_ptr<Shared> shared;

        weak_thread_ref_t* Node;
        int context_level;

        std::uint8_t pad_0[12];

        std::uintptr_t script_context;
        std::uintptr_t unk_0;
        std::uintptr_t context_perms;
        std::uintptr_t unk_1;
        std::weak_ptr<uintptr_t> script;

        void* script_ref_1;
        void* script_ref_2;
        void* script_ref_3;
        std::uintptr_t script_parent;
        std::uintptr_t script_parent_ref;
    };

    enum rbx_ptypes : uint8_t
    {
        print,
        info,
        warn,
        error
    };
}

namespace funcs {
    extern lua_State* (*getmainstate)(lua_State* rL);
    extern int (*scriptcontextresume)(int unk, std::uintptr_t sc, std::uintptr_t* ref, int nargs, int, int);

    void init();
}