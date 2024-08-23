#include <jni.h>
#include <errno.h>

#include <string>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>

// Maybe add debug to enable and disable this?

#define TAG "AndroidCore-V1"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))

// Module Configuration here for easier access.

namespace exploit_configuration
{
	const std::string exploit_name = "AndroidCore";
	const std::string exploit_version = "1.0.0";
	const std::string lua_chunk = "@AC";
}