
#include "Camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/string_cast.hpp>

static float gSens{ 0.5f };
static float gSpeed{ 5.f }; // movement speed

namespace nxt
{

	Camera::Camera(const glm::vec3& position, const glm::vec3& lookAtPosition) :
		mPosition{ position },
		mDirection{ glm::normalize(lookAtPosition - position) },
		mMousePosition{ input::GetMousePosition() }
	{
		mYaw = -glm::degrees(std::acos(mDirection.x));
		mPitch = glm::degrees(std::asin(mDirection.y));

		BuildViewMatrix();
		BuildProjectionMatrix();
		BuildProjectionViewMatrix();
	}

	void Camera::OnUpdate(float dt)
	{
		if (input::IsKeyDown(input::KEYCODE_MOUSE_RIGHT))
		{
			// if usable check
			glm::vec2 mousePosition{ input::GetMousePosition() };
			glm::vec2 mouseDelta{ mMousePosition - mousePosition };
			mMousePosition = mousePosition;
			Rotate(dt, mouseDelta);
			Translate(dt);
			BuildProjectionViewMatrix();
		}
	}

	bool Camera::OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowResized>(NXT_CALLBACK(Camera::OnWindowResize));
		handler.Fire<events::MouseButtonPressed>(NXT_CALLBACK(Camera::OnMouseButtonPressed));
		handler.Fire<events::MouseButtonReleased>(NXT_CALLBACK(Camera::OnMouseButtonReleased));
		return false;
	}

	bool Camera::OnMouseButtonPressed(events::MouseButtonPressed& ev)
	{
		if (ev.Keycode == input::KEYCODE_MOUSE_RIGHT)
		{
			mMousePosition = input::GetMousePosition();
			input::HideCursor();
		}
		return false;
	}

	bool Camera::OnMouseButtonReleased(events::MouseButtonReleased& ev)
	{
		if (ev.Keycode == input::KEYCODE_MOUSE_RIGHT)
		{
			input::ShowCursor();
		}
		return false;
	}

	bool Camera::OnWindowResize(events::WindowResized& ev)
	{
		mAspectRatio = static_cast<float>(ev.Width) / static_cast<float>(ev.Height);
		BuildProjectionMatrix();
		BuildProjectionViewMatrix();
		return false;
	}

	void Camera::BuildViewMatrix()
	{
		mViewMatrix = glm::lookAt(mPosition, mPosition + GetLookVector(), GetUpVector());
	}

	void Camera::BuildProjectionMatrix()
	{
		mProjectionMatrix = glm::perspective(glm::radians(mFov), mAspectRatio, 0.1f, 1000.f);
	}

	void Camera::BuildProjectionViewMatrix()
	{
		mProjectionViewMatrix = mProjectionMatrix * mViewMatrix;
	}

	void Camera::Rotate(float& dt, glm::vec2& mouseDelta)
	{
		mYaw -= mouseDelta.x * gSens;
		mPitch = std::max(-89.0f, std::min(89.0f, mPitch + (mouseDelta.y * gSens)));

		glm::vec3 direction{};
		direction.x = std::cos(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));
		direction.y = std::sin(glm::radians(mPitch));
		direction.z = std::sin(glm::radians(mYaw)) * std::cos(glm::radians(mPitch));
		mDirection = glm::normalize(direction);

		BuildProjectionMatrix();
	}

	void Camera::Translate(float& dt)
	{
		// Horizontal
		if (input::IsKeyDown(input::KEYCODE_A))
		{
			mPosition += dt * GetRightVector();
		}
		else if (input::IsKeyDown(input::KEYCODE_D))
		{
			mPosition -= dt * GetRightVector();
		}
		// Forward
		if (input::IsKeyDown(input::KEYCODE_W))
		{
			mPosition += dt * GetLookVector();
		}
		else if (input::IsKeyDown(input::KEYCODE_S))
		{
			mPosition -= dt * GetLookVector();
		}
		// Vertical
		if (input::IsKeyDown(input::KEYCODE_E))
		{
			mPosition += dt * GetUpVector();
		}
		else if (input::IsKeyDown(input::KEYCODE_Q))
		{
			mPosition -= dt * GetUpVector();
		}
		BuildViewMatrix();
	}

	glm::vec3 Camera::GetRightVector()
	{
		return glm::normalize(glm::cross(glm::vec3{ 0.f, 1.f, 0.f }, mDirection));
	}

	glm::vec3 Camera::GetUpVector()
	{
		return glm::normalize(glm::cross(mDirection, GetRightVector()));
	}

	glm::vec3 Camera::GetLookVector()
	{
		// mDirection is already normalized
		return mDirection;
	}

	glm::mat4& Camera::GetViewMatrix()
	{
		return mViewMatrix;
	}

	glm::mat4& Camera::GetProjectionMatrix()
	{
		return mProjectionMatrix;
	}

	glm::mat4& Camera::GetProjectionViewMatrix()
	{
		return mProjectionViewMatrix;
	}

}
