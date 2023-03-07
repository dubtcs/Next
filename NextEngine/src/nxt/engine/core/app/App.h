#pragma once

#include <nxt/engine/EngineCore.h>
#include "../window/Window.h"

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	class App
	{
	public:
		bool OnEvent(events::Event& ev);
		void Run();
		App();
		~App();
	protected:
		bool OnWindowClose(events::WindowClosed& ev);

	protected:
		bool mRunning{ true };
		Shared<Window> mWindow;
	};

}

#else

#error "Windows only"

#endif
