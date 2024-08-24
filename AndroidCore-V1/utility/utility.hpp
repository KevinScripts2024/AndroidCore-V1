#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <sstream>

namespace utility {
    const std::string randomStr(std::size_t length);

    const std::string randomBytes(std::size_t length);

    namespace memory {
        std::uintptr_t findLib(const char* const library);
        void waitForLib(const char* const libname);
        std::uintptr_t rebase(const char* const libraryName, std::uintptr_t relativeAddr);
        bool isLibLoaded(const char* const libraryName);
    }

    namespace JNI {
        extern JNIEnv* env;
        extern jobject GlobalContext;

        jobject getGlobalContext();

        void setClipboardData(const std::string& text);
    }
}