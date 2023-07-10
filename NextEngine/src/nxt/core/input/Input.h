#pragma once

#include <nxt/EngineCore.h>
#include "InputEnums.h"

#include <glm/glm.hpp>

namespace nxt::input
{

	bool IsKeyDown(nxtKeycode keycode);
	glm::vec2 GetMousePosition();
	glm::vec2 GetMouseScreenPosition();

	//void ToggleCursor(bool show);
	void ShowCursor();
	void HideCursor();
	bool SetMouseScreenPosition(glm::vec2 newPosition); // Screen space coordinates

}
