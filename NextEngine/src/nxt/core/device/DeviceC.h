#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/input/InputEnums.h>
#include <nxt/core/window/Window.h>
#include <glm/glm.hpp>
#include <list>
#include <vector>

namespace nxt
{

	class NXT_API DeviceC
	{
	public:
		DeviceC(bool createWindow = true);
		bool IsKeyDown(nxtKeycode keycode) const;
		glm::vec2 GetMousePosition() const;
		bool CreateRenderThread() const;
		bool ShowCursor() const;
		bool HideCursor() const;
	protected:
		std::vector<SWindow> mWindows{};
	};

}
