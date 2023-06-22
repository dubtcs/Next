#pragma once

#include "Interface.h"

#include <nxt/EngineCore.h>
#include <string>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	namespace app
	{
		void Launch(const std::string& appName);
		void Run();
		bool OnEvent(events::Event& ev);
		void AddInterface(const Shared<AppInterface>& appInterface);
	}

}

#else

#error "Windows only"

#endif
