#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt::events
{

	enum EVENT_TYPE_ : nxt_enum
	{
		EVENT_TYPE_NONE = BIT(0),
		EVENT_TYPE_MOUSE = BIT(1),
		EVENT_TYPE_MOUSE_MOVE = BIT(2),
		EVENT_TYPE_MOUSE_PRESSED = BIT(3),
		EVENT_TYPE_MOUSE_RELEASED = BIT(4),
		EVENT_TYPE_KEYBOARD = BIT(5),
		EVENT_TYPE_WINDOW_RESIZE = BIT(6),
		EVENT_TYPE_WINDOW_CLOSE = BIT(7)
	};

}
