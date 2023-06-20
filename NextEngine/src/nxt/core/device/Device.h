#pragma once

#include <nxt/EngineCore.h>

/*
Device are platform specific viewport/window commands
*/

namespace nxt::device
{

#ifdef NXT_PLATFORM_WINDOWS
	bool Init(HWND* windowHandle);
	bool Release();
	void SwapBuffers();
#else

#error "windows"

#endif

}
