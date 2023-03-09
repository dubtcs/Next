#pragma once

#include <nxt/engine/EngineCore.h>

#include "nxt/engine/core/events/Event.h"
#include "nxt/engine/render/device/GLRender.h"
#include <functional>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	class NXT_API Window
	{
	public:
		bool Init();
		bool Release();
		bool ProcessMessages();
		bool OnCreate();
		bool OnUpdate(float dt);
		bool OnClose();
		bool OnResize(uint32_t x, uint32_t y);
		void SetEventCallback(std::function<bool(events::Event& ev)> func);
		Window();
		Window(std::function<bool(events::Event&)>);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	protected:
		HWND mWindowHandle;
		HINSTANCE mHinstance;

		// rendering contexts
		/*HDC mDeviceContext;
		HGLRC mRenderingContext;*/

		std::function<bool(events::Event&)> mCallback;
	};

}

#else
#error "Windows only"

namespace nxt
{

	class AXT_API Window
	{
		// Mac/Linux stuff
	};

}

#endif
