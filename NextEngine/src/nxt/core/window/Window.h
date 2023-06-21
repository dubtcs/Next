#pragma once

#include <nxt/EngineCore.h>

#include "nxt/core/events/Event.h"
#include "nxt/core/device/Device.h"
#include <functional>

#ifdef NXT_PLATFORM_WINDOWS

namespace nxt
{

	class NXT_API Window
	{
	public:
		bool Init(const std::string& windowTitle = "NxtWindow");
		bool Release();
		bool ProcessMessages();
		bool OnCreate();
		bool OnUpdate(float dt);
		bool OnClose();
		bool OnResize(uint32_t x, uint32_t y);
		void SetEventCallback(std::function<bool(events::Event& ev)> func);
		Window();
		Window(const std::string& windowTitle, std::function<bool(events::Event&)>);
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
