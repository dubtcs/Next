
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

static float gSens{ 0.5f };
static float gSpeed{ 50.f }; // movement speed

static constexpr float gScrollModifier{ 0.1f };

static constexpr float gNearClip{ 0.01f };
static constexpr float gFarClip{ 10'000.f };

namespace nxt
{

	Camera::Camera(const glm::vec3& position, const glm::vec3& lookAtPosition) :
		mPosition{ position },
		mDirection{ glm::normalize(lookAtPosition - position) },
		mMousePosition{ input::GetMouseScreenPosition() }
	{
		mYaw = -glm::degrees(std::acos(mDirection.x));
		mPitch = glm::degrees(std::asin(mDirection.y));

		BuildViewMatrix();
		BuildProjectionMatrix();
		BuildProjectionViewMatrix();
	}

	void Camera::OnUpdate(float dt)
	{
		if (input::IsKeyDown(nxtKeycode_MouseRight))
		{
			// if usable check
			glm::vec2 mousePosition{ input::GetMouseScreenPosition() };
			glm::vec2 mouseDelta{ mMousePosition - mousePosition };
			//mMousePosition = mousePosition;
			Rotate(dt, mouseDelta);
			Translate(dt);
			BuildProjectionViewMatrix();
			input::SetMouseScreenPosition(mMousePosition);
		}
	}

	bool Camera::OnEvent(events::Event& ev)
	{
		events::Handler handler{ ev };
		handler.Fire<events::WindowResized>(NXT_CALLBACK(Camera::OnWindowResize));
		handler.Fire<events::MouseButtonPressed>(NXT_CALLBACK(Camera::OnMouseButtonPressed));
		handler.Fire<events::MouseButtonReleased>(NXT_CALLBACK(Camera::OnMouseButtonReleased));
		handler.Fire<events::MouseScroll>(NXT_CALLBACK(Camera::OnMouseScroll));
		return false;
	}

	bool Camera::OnMouseButtonPressed(events::MouseButtonPressed& ev)
	{
		if (ev.Keycode == nxtKeycode_MouseRight)
		{
			mMousePosition = input::GetMouseScreenPosition();
			input::HideCursor();
		}
		return false;
	}

	bool Camera::OnMouseButtonReleased(events::MouseButtonReleased& ev)
	{
		if (ev.Keycode == nxtKeycode_MouseRight)
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

	bool Camera::OnMouseScroll(events::MouseScroll& ev)
	{
		float adjusted{ ev.delta * gScrollModifier };
		gSpeed = std::max(1.f, gSpeed + adjusted);
		return false;
	}

	void Camera::BuildViewMatrix()
	{
		mViewMatrix = glm::lookAt(mPosition, mPosition + GetLookVector(), GetUpVector());
	}

	void Camera::BuildProjectionMatrix()
	{
		mProjectionMatrix = glm::perspective(glm::radians(mFov), mAspectRatio, gNearClip, gFarClip);
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
		if (input::IsKeyDown(nxtKeycode_A))
		{
			mPosition += gSpeed * dt * GetRightVector();
		}
		else if (input::IsKeyDown(nxtKeycode_D))
		{
			mPosition -= gSpeed * dt * GetRightVector();
		}
		// Forward
		if (input::IsKeyDown(nxtKeycode_W))
		{
			mPosition += gSpeed * dt * GetLookVector();
		}
		else if (input::IsKeyDown(nxtKeycode_S))
		{
			mPosition -= gSpeed * dt * GetLookVector();
		}
		// Vertical
		if (input::IsKeyDown(nxtKeycode_E) || input::IsKeyDown(nxtKeycode_Space))
		{
			mPosition += gSpeed * dt * GetUpVector();
		}
		else if (input::IsKeyDown(nxtKeycode_Q) || input::IsKeyDown(nxtKeycode_Ctrl))
		{
			mPosition -= gSpeed * dt * GetUpVector();
		}
		BuildViewMatrix();
	}

	glm::vec3 Camera::GetPosition()
	{
		return mPosition;
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
