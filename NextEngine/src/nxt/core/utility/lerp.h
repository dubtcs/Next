#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtEasingStyle : nxt_enum
	{
		nxtEasingStyle_Linear = BIT(1),
		nxtEasingStyle_Exponential = BIT(2)
	};

	float Lerp(float start, float target, float timestep, nxtEasingStyle easingStyle = nxtEasingStyle_Linear);

}
