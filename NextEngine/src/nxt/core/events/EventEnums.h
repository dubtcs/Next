#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt::events
{

	enum nxtEventType : nxt_enum
	{
		nxtEventType_None = BIT(0),
		nxtEventType_Mouse = BIT(1),
		nxtEventType_MouseMove = BIT(2),
		nxtEventType_MousePressed = BIT(3),
		nxtEventType_MouseReleased = BIT(4),
		nxtEventType_Keyboard = BIT(5),
		nxtEventType_WindowResize = BIT(6),
		nxtEventType_WindowClose = BIT(7)
	};

}
