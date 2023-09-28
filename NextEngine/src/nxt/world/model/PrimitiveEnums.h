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

	enum nxtMorphTarget : nxt_enum
	{
		nxtMorphTarget_Position = BIT(0),
		nxtMorphTarget_Normal = BIT(1),
		nxtMorphTarget_Tangent = BIT(2),
		nxtMorphTarget_TexturePosition = BIT(3)
	};

}
