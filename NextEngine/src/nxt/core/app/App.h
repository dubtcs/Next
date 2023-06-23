#pragma once

#include "Interface.h"

#include <nxt/EngineCore.h>
#include <string>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	namespace app
	{
		void Launch(const std::string& appName, int32_t width = 1920, int32_t height = 1080);
		void Run();
		bool OnEvent(events::Event& ev);
		void AddInterface(const Shared<AppInterface>& appInterface);
	}

}

#else

#error "Windows only"

#endif
