#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace nxt::cmp
{
	struct Light
	{
		glm::vec3 Position;		// 0 Offset, 16 byte alignement
		float Intensity;		// 12 Offset, 4 Byte alignment
		glm::vec3 Direction;	// 16 Offset, 16 byte alignment
		int32_t LightType;		// 28 Offset, 4 byte alignment
		glm::vec3 Color;		// 32 Offset, 16 byte alignment
		float Radius;			// 44 Offset, 4 byte alignment
	}; // Total 48 bytes, 16 byte alignment
}
