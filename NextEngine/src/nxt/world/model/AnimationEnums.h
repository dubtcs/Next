#pragma once

#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtAnimationFlag : nxt_enum
	{
		nxtAnimationFlag_Loop = BIT(0),
		nxtAnimationFlag_Complete = BIT(1),
		nxtAnimationFlag_Paused = BIT(2)
	};

	enum nxtAnimationTarget : nxt_enum
	{
		nxtAnimationTarget_Position = 0,
		nxtAnimationTarget_Rotation = 1,
		nxtAnimationTarget_Scale = 2,
		nxtAnimationTarget_Weights = 3,

		// 10-19 color targets
		nxtAnimationTarget_Color0 = 10,

		// 20-29 texture positions
		nxtAnimationTarget_TexCoord0 = 20

	};

}
