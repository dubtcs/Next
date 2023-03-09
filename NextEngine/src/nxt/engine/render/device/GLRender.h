#pragma once

#include <nxt/engine/EngineCore.h>

namespace nxt::render
{

#ifdef NXT_PLATFORM_WINDOWS
	bool Init(HWND* windowHandle);
	bool Release();
	void FrameStart();
	void FrameEnd();
	//void SetClearColor(float& r, float& g, float& b);
#else
#error "windows"
#endif

}
