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
		bool OnKeyPressed(events::KeyboardPressed& ev);
	protected:
		Camera mCamera;

		uint32_t mWidth;
		uint32_t mHeight;

		buffers::SFrameBuffer mFrameBuffer;

		Shader mShader;
		Shader mSkyboxShader;
		Shader mNormalShader;

		STexture mWood;

		SCubemap mSkyboxCubemap;
		buffers::SDataBuffer mMatrixBuffer;
	};

}
