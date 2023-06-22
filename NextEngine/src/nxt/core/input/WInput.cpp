
#include "Input.h"

#include <nxt/EngineCore.h>
#include <nxt/core/app/App.h>

#include <nxt/core/device/Device.h>

namespace nxt::input
{
#ifdef NXT_PLATFORM_WINDOWS

	bool IsKeyDown(KEYCODE_ keycode)
	{
		return device::IsKeyDown(keycode);
	}

#endif
}