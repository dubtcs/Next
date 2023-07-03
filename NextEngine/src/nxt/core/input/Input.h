#pragma once

#include <nxt/EngineCore.h>
#include "InputEnums.h"

#include <glm.hpp>

namespace nxt::input
{

	bool IsKeyDown(nxtKeycode keycode);
	glm::vec2 GetMousePosition();

	//void ToggleCursor(bool show);
	void ShowCursor();
	void HideCursor();

}
