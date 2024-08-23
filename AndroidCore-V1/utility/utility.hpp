#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <sstream>

namespace memory {
    std::uintptr_t findLib(const char* const library);
    void waitForLib(const char* const libname);
    std::uintptr_t rebase(const char* const libraryName, std::uintptr_t relativeAddr);
    bool isLibLoaded(const char* const libraryName);
}