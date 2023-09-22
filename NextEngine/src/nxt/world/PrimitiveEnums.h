#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtPrimitiveFeature : nxt_enum
	{
		nxtPrimitiveFeature_Normals = BIT(0),
		nxtPrimitiveFeature_Tangents = BIT(1)
	};

}
