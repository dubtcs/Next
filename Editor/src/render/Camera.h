#pragma once

#include <NextEngine.h>
#include <glm.hpp>

namespace nxt
{

	class Camera
	{
	public:
		Camera(const glm::vec3& position = { 0.f, 1.f, 3.f }, const glm::vec3& lookAtPosition = { 0.f, 0.f, 0.f });
		void OnUpdate(float dt);
		bool OnEvent(events::Event& ev);
		glm::vec3 GetLookVector();
		glm::vec3 GetRightVector();
		glm::vec3 GetUpVector();

		glm::mat4& GetViewMatrix();
		glm::mat4& GetProjectionMatrix();
		glm::mat4& GetProjectionViewMatrix();
	protected:
		void Translate(float& dt);
		void Rotate(float& dt, glm::vec2& mouseDelta);
		void BuildViewMatrix();
		void BuildProjectionMatrix();
		void BuildProjectionViewMatrix();
		bool OnWindowResize(events::WindowResized& ev);
		bool OnMouseButtonPressed(events::MouseButtonPressed& ev);
		bool OnMouseButtonReleased(events::MouseButtonReleased& ev);
	protected:
		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec2 mMousePosition;

		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mProjectionViewMatrix;

		float mYaw{ 0.f };
		float mPitch{ 0.f };

		float mFov{ 90.f };
		float mAspectRatio{ 0.f };

		bool mIsAiming{ false };
	};

}
