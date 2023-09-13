#pragma once

#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtAnimationTarget : nxt_enum
	{
		nxtAnimationTarget_Position = 0,
		nxtAnimationTarget_Rotation = 1,
		nxtAnimationTarget_Scale = 3,

		// 10-19 color targets
		nxtAnimationTarget_Color0 = 10,

		// 20-29 texture positions
		nxtAnimationTarget_TexCoord0 = 20

	};

}
