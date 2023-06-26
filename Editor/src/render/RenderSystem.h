#pragma once

#include <NextEngine.h>

#include "Camera.h"

namespace nxt
{

	class RenderSystem
	{
	public:
		RenderSystem();
		void OnUpdate(float& dt);
		bool OnEvent(events::Event& ev);
	protected:
		bool OnWindowResize(events::WindowResized& ev);
	protected:
		Camera mCamera;

		Shared<buffers::VertexBuffer> mVertexBuffer;
		Shared<buffers::ElementBuffer> mElementBuffer;

		Shader mShader;

		Shared<texture::Texture> mTexture;
	};

}
