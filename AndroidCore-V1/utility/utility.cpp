#include "utility.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <chrono>
#include <unistd.h>
#include <dlfcn.h>

namespace utility {
    const std::string randomStr(std::size_t length) {
        static const char alphabet[] =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789";

        std::random_device rd;
        std::default_random_engine rng(rd());
        std::uniform_int_distribution<> dist(0, sizeof(alphabet) / sizeof(*alphabet) - 2);

        std::string str;
        str.reserve(length);
        std::generate_n(std::back_inserter(str), length,
            [&]() { return alphabet[dist(rng)]; });

        return str;
    }

    const std::string randomBytes(std::size_t length) {
        static const char alphabet[] =
            "ABCDEF"
            "0123456789";

        std::random_device rd;
        std::default_random_engine rng(rd());
        std::uniform_int_distribution<> dist(0, sizeof(alphabet) / sizeof(*alphabet) - 2);

        std::string str;
        str.reserve(length);
        std::generate_n(std::back_inserter(str), length,
            [&]() { return alphabet[dist(rng)]; });

        return str;
    }

    namespace memory {
        std::uintptr_t findLib(const char* const library) {
            char filename[0xFF] = { 0 }, buffer[1024] = { 0 };

            FILE* fp = nullptr;
            std::uintptr_t address = 0;

            snprintf(filename, sizeof(filename), "/proc/self/maps");

            fp = fopen(filename, "rt");

            if (fp == nullptr) {
                perror("fopen");
                goto done;
            }

            while (fgets(buffer, sizeof(buffer), fp)) {
                if (strstr(buffer, library)) {
                    address = static_cast<std::uintptr_t>(strtoul(buffer, NULL, 16));
                    goto done;
                }
            }

        done:

            if (fp)
                fclose(fp);

            return address;
        }

        void waitForLib(const char* const libname) {
            while (!isLibLoaded(libname))
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::uintptr_t rebase(const char* const libraryName, std::uintptr_t relativeAddr) {
            waitForLib(libraryName);
            std::uintptr_t libBase = findLib(libraryName);

            if (libBase == 0)
                return 0;

            return libBase + relativeAddr;
        }

        bool isLibLoaded(const char* const libraryName) {
            char line[512] = { 0 };

            FILE* fp = std::fopen("/proc/self/maps", "rt");

            if (fp != nullptr) {
                while (std::fgets(line, sizeof(line), fp)) {
                    if (std::strstr(line, libraryName))
                        return true;
                }

                fclose(fp);
            }

            return false;
        }
    }
}