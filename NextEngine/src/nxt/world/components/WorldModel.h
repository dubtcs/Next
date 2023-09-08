#pragma once

#include "../Model.h"
#include "../Model2.h"
#include <glm/glm.hpp>

namespace nxt::cmp
{
	// Shared pointer to a struct of a shared model pointer.
	// Allows all components with a certain model to be swapped at once or individually

	struct WorldModel
	{
		//SModelInstance Instance{ nullptr };
		Shared<Model2Instance> Instance{ nullptr };
		WorldModel() = default;
		//WorldModel(const SModelInstance& instance) : Instance{ instance } {}
		WorldModel(const Shared<Model2Instance>& instance) : Instance{ instance } {}
	};
}
