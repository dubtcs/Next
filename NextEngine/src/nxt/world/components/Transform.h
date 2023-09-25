#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nxt::cmp
{
	struct Transform
	{
		glm::vec3 Position{ 0.f };
		glm::vec4 Rotation{ 0.f };
		glm::vec3 Scale{ 1.f };

		glm::mat4 ToMatrix()
		{
			glm::mat4 ones{ 1.f };
			return glm::translate(ones, Position) * glm::toMat4(glm::quat{ Rotation.w, Rotation.x, Rotation.y, Rotation.z }) * glm::scale(ones, Scale);
		}
	};
}
