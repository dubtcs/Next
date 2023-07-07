#pragma once

#include <glm/glm.hpp>

namespace nxt::cmp
{
	struct Light
	{
		float Intensity;
		glm::vec3 Color;
		glm::vec3 Direction;
	};
}
