#include <thread>
#include <inttypes.h>
#include "pch.hpp"


void handler(int sig)
{
	LOGE("[Handler] Caught a crash, expected signal at {%d}", sig);
}

// ^^ Crash handler
// JNI and other memory should be handled here