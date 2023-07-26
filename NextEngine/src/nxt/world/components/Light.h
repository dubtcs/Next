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
		glm::vec3 Position{ 0.f };							// 0 Offset, 16 byte alignement
		float Intensity{ 1.f };								// 12 Offset, 4 Byte alignment
		glm::vec3 Direction{ 0.f };							// 16 Offset, 16 byte alignment
		nxtLightType LightType{ nxtLightType_Ambient };		// 28 Offset, 4 byte alignment
		glm::vec3 Color{ 1.f };								// 32 Offset, 16 byte alignment
		float Radius{ 25.f };								// 44 Offset, 4 byte alignment
	}; // Total 48 bytes, 16 byte alignment
}
