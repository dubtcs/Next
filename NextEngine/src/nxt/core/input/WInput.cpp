
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