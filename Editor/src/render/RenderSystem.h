#pragma once

#include <NextEngine.h>

#include "Camera.h"

#include <nxt/world/Model.h>

namespace nxt
{

	class RenderSystem
	{
	public:
		RenderSystem();
		void OnUpdate(float& dt, World& world);
		bool OnEvent(events::Event& ev);
	protected:
		bool OnWindowResize(events::WindowResized& ev);
	protected:
		Camera mCamera;

		Shared<buffers::VertexBuffer> mVertexBuffer;
		Shared<buffers::ElementBuffer> mElementBuffer;

		Shader mShader;
		Shader mSkyboxShader;

		SCubemap mSkyboxCubemap;
	};

}
