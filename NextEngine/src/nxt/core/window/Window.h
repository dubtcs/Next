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
		bool Init(const std::string& windowTitle = "NxtWindow", int32_t width = 1920, int32_t height = 1080);
		bool Release();
		bool ProcessMessages();
		bool OnCreate();
		bool OnUpdate(float dt);
		bool OnEvent(events::Event& ev);
		void SetEventCallback(std::function<bool(events::Event& ev)> func);
		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		Window();
		Window(const std::string& windowTitle = "NxtWindow", int32_t width = 1920, int32_t height = 1080);
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

		//uint32_t mTrueWidth{ 0 };
		//uint32_t mTrueHeight{ 0 };

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
