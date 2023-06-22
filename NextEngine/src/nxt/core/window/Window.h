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
		bool OnEvent(events::Event& ev);
		bool OnClose(events::WindowClosed& ev);
		void SetEventCallback(std::function<bool(events::Event& ev)> func);
		Window();
		Window(const std::string& windowTitle, std::function<bool(events::Event&)>);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	protected:
		bool OnResize(events::WindowResized& ev);
		bool OnKeyPressed(events::KeyboardPressed& ev);
	public:
		HWND mWindowHandle;
		HINSTANCE mHinstance;

		uint32_t mWidth{ 0 };
		uint32_t mHeight{ 0 };

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
