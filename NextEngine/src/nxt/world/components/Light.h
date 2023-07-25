#pragma once

#include <glm/glm.hpp>
#include <memory>

#include <nxt/core/GenericEnums.h>

namespace nxt::cmp
{

	enum nxtLightType : nxt_enum
	{
		nxtLightType_Point = 0,
		nxtLightType_Directional = 1,
		nxtLightType_Spot = 2,
		nxtLightType_Ambient = 3,
	};

	struct Light
	{
		glm::vec3 Position;		// 0 Offset, 16 byte alignement
		float Intensity;		// 12 Offset, 4 Byte alignment
		glm::vec3 Direction;	// 16 Offset, 16 byte alignment
		nxtLightType LightType;		// 28 Offset, 4 byte alignment
		glm::vec3 Color;		// 32 Offset, 16 byte alignment
		float Radius;			// 44 Offset, 4 byte alignment
	}; // Total 48 bytes, 16 byte alignment
}
