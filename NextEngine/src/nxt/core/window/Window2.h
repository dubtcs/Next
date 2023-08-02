#pragma once

#include <nxt/EngineCore.h>

namespace nxt
{
#ifdef NXT_PLATFORM_WINDOWS
	class NXT_API Window2
	{
	public:
		Window2();
		void Init(const std::string& windowTitle = "Nxt Window", int32_t width = 1920, int32_t height = 1080);
		void Release();
		void CreateRenderContext();
		bool ProcessMessages();
	protected:
		HWND mWindowHandle;
		HINSTANCE mHInstance;
		int32_t mWidth;
		int32_t mHeight;
		bool isFocused{ false };
	public:
		friend LRESULT CALLBACK NxtWindowProcedure2(HWND, UINT, WPARAM, LPARAM);
	};
#endif
}
