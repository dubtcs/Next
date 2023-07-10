#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/input/InputEnums.h>

#include <glm/glm.hpp>

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
	bool IsKeyDown(nxtKeycode keycode);
	glm::vec2 GetMousePosition(); // Returns window space coordinates
	glm::vec2 GetMouseScreenPosition();
	int SetCursorPosition(glm::vec2 newPosition); // Screen space coordinates
	void ShowCursor();
	void HideCursor();
#else

#error "windows"

#endif

}
