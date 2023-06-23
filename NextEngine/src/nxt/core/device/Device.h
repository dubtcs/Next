#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/input/InputEnums.h>

#include <glm.hpp>

/*
Device are platform specific viewport/window commands
Ideally, this isn't exposed in the engine.
This is strictly for engine cpp files to use.
*/

namespace nxt::device
{

#ifdef NXT_PLATFORM_WINDOWS
	bool Init(HWND* windowHandle);
	bool Release();
	void SwapBuffers();
	bool IsKeyDown(input::KEYCODE_ keycode);
	glm::vec2 GetMousePosition();
	void ShowCursor();
	void HideCursor();
#else

#error "windows"

#endif

}
