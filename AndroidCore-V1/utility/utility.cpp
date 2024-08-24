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

namespace utils {
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

    namespace JNI {
        JNIEnv* env = nullptr;
        jobject GlobalContext = nullptr;

        jobject getGlobalContext() {
            jclass activityThread = env->FindClass("android/app/ActivityThread");
            jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
            jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);

            jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
            jobject context = env->CallObjectMethod(at, getApplication);
            return context;
        }

        void setClipboardData(const std::string& text) {
            if (!GlobalContext) {
                std::cerr << " [ JNI ERR ] Failed to get Context. wtf?" << std::endl;
                exit(999);
            }

            auto contextClass = env->FindClass("android/content/Context");
            if (contextClass == nullptr) {
                std::cerr << "[ JNI ERR ] contextClass was nullptr" << std::endl;
                return;
            }

            auto clipboardManagerClass = env->FindClass("android/content/ClipboardManager");
            if (clipboardManagerClass == nullptr) {
                std::cerr << "[ JNI ERR ] clipboardManagerClass was nullptr" << std::endl;
                return;
            }

            auto clipDataClass = env->FindClass("android/content/ClipData");
            if (clipDataClass == nullptr) {
                std::cerr << "[ JNI ERR ] clipDataClass was nullptr" << std::endl;
                return;
            }

            auto newPlainTextMethod = env->GetStaticMethodID(clipDataClass, "newPlainText", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;");
            auto setPrimaryClipMethod = env->GetMethodID(clipboardManagerClass, "setPrimaryClip", "(Landroid/content/ClipData;)V");
            auto getSystemServiceMethod = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

            auto clipboardService = env->CallObjectMethod(GlobalContext, getSystemServiceMethod, env->NewStringUTF("clipboard"));

            auto data = env->NewStringUTF(text.c_str());
            auto clip = env->CallStaticObjectMethod(clipDataClass, newPlainTextMethod, env->NewStringUTF("MBClip"), data);
            env->CallVoidMethod(clipboardService, setPrimaryClipMethod, clip);
        }
    }
}