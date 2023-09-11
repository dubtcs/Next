#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/GenericEnums.h>

namespace nxt
{

	float Lerp(float start, float target, float timestep, nxtInterpolation easingStyle = nxtInterpolation_Linear);

}
