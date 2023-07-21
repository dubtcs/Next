#pragma once

#include <NextEngine.h>

#include "Camera.h"

#include <nxt/world/Model.h>
#include <nxt/render/texture/Shadowmap2.h>

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

		STexture mWood;

		buffers::SDataBuffer mFrameInfoBuffer;
		buffers::SDataBuffer mLightInfoBuffer;
		buffers::SDataBuffer mObjectInfoBuffer;
		buffers::SDataBuffer mMaterialInfoBuffer;

		Shadowmap2 mShadowmap;
		Shader mShadowShader;
	};

}
