#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/input/InputEnums.h>

#include <glm/glm.hpp>

namespace nxt
{
	class NXT_API Device
	{
#ifdef NXT_PLATFORM_WINDOWS
	public:
		Device(HWND* windowHandle);
		bool Release();
		void SwapBuffers();
		void SetDropTarget(bool enabled);
	protected:
		bool mAcceptingDrops{ false };
		HWND* mWindowHandle{ nullptr };
		HGLRC mRenderContext;
		//static HDC mDeviceContext; // Move into cpp file. Only need one device context
#endif
	};

}
