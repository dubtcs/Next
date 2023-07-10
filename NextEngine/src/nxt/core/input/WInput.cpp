
#include "Input.h"

#include <nxt/EngineCore.h>
#include <nxt/core/app/App.h>

#include <nxt/core/device/Device.h>

namespace nxt::input
{
#ifdef NXT_PLATFORM_WINDOWS

	bool IsKeyDown(nxtKeycode keycode)
	{
		return device::IsKeyDown(keycode);
	}

	glm::vec2 GetMousePosition()
	{
		return device::GetMousePosition();
	}

	glm::vec2 GetMouseScreenPosition()
	{
		return device::GetMouseScreenPosition();
	}

	bool SetMouseScreenPosition(glm::vec2 pos)
	{
		return static_cast<bool>(device::SetCursorPosition(pos));
	}

	void ShowCursor()
	{
		device::ShowCursor();
	}

	void HideCursor()
	{
		device::HideCursor();
	}

#endif
}