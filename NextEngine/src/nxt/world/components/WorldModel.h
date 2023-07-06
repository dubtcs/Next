#pragma once

#include "../Model.h"
#include <glm/glm.hpp>

namespace nxt::cmp
{
	struct WorldModel
	{
		glm::vec4 Color{ 0.5f, 0.5f, 0.5f, 1.f };
		glm::vec3 Size{ 1.f };
		Shared<Model> ModelInstance;
		WorldModel(Shared<Model>& model, const glm::vec3& size = glm::vec3{ 1.f }, const glm::vec4& color = glm::vec4{ 1.f }) : 
			ModelInstance{ model }, Color { color }, Size{ size } {}
		WorldModel() = default;
	};
}
