#pragma once

#include <NextEngine.h>

#include "Camera.h"

#include <nxt/world/Model.h>
#include <nxt/render/texture/PointShadowmap.h>

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
		Shader mShadowShader;

		STexture mWood;

		Shared<PointShadowmap> mShadowmap;

		buffers::SDataBuffer mCameraMatrixBuffer;
		buffers::SDataBuffer mLightInfoBuffer;

	};

}
