#pragma once

#include <nxt/EngineCore.h>

/*
I'm only using OpenGL atm, so having a namespace for the renderer instead of a class makes more sense to me.
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
