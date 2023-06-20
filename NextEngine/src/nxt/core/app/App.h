#pragma once

#include <nxt/EngineCore.h>
#include "../window/Window.h"

#include <nxt/render/buffers/VertexBuffer.h>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	class NXT_API App
	{
	public:
		bool OnEvent(events::Event& ev);
		void Run();
		App();
		~App();
	protected:
		bool OnWindowClose(events::WindowClosed& ev);
		bool OnWindowResize(events::WindowResized& ev);
	protected:
		bool mRunning{ true };
		Shared<Window> mWindow;
	};

}

#else

#error "Windows only"

#endif
