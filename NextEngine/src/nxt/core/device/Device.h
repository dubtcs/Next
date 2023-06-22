#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/input/InputEnums.h>

/*
Device are platform specific viewport/window commands
*/

namespace nxt::device
{

#ifdef NXT_PLATFORM_WINDOWS
	bool Init(HWND* windowHandle);
	bool Release();
	void SwapBuffers();
	bool IsKeyDown(input::KEYCODE_ keycode);
#else

#error "windows"

#endif

}
