
#include "Camera.h"

#include <nxt/core/input/Input.h>

namespace nxt
{

	Camera::Camera(const glm::vec3& position) :
		mPosition{ position }
	{

	}

	void Camera::OnUpdate(double dt)
	{
		if (input::IsKeyDown(input::KEYCODE_MOUSE_RIGHT))
		{
			Translate(dt);
			//Rotate(dt);
		}
	}

	void Camera::Translate(double& dt)
	{

	}

	void Camera::Rotate(double& dt, glm::vec2& mouseDelta)
	{

	}

}
