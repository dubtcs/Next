#pragma once

#include <nxt/EngineCore.h>
#include <glm.hpp>

namespace nxt
{

	class NXT_API Camera
	{
	public:
		Camera(const glm::vec3& position = { 0.f, 0.f, 0.f });
		void OnUpdate(double dt);
	protected:
		void Translate(double& dt);
		void Rotate(double& dt, glm::vec2& mouseDelta);
	protected:
		glm::vec3 mPosition;
		glm::vec3 mDirection;
	};

}
