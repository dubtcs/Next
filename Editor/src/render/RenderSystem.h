#pragma once

#include <NextEngine.h>

#include "Camera.h"

//#include <nxt/render/texture/Shadowmap2.h>
#include <nxt/render/ScreenQuad.h>

namespace nxt
{

	class RenderSystem
	{
	public:
		RenderSystem();
		void OnUpdate(float& dt, World& world, bool isFocused);
		bool OnEvent(events::Event& ev);
		void SetViewModel(const cmp::WorldModel& modelref);
	protected:
		bool OnWindowResize(events::WindowResized& ev);
		bool OnKeyPressed(events::KeyboardPressed& ev);
		void BuildDeferredBuffer();
	protected:
		Camera mCamera;

		uint32_t mWidth;
		uint32_t mHeight;

		SFrameBuffer mBuffer2;
		SFrameBuffer mDeferredBuffer;
		SFrameBuffer mSSAO;
		SFrameBuffer mAOBlur;
		SFrameTexture mNoise;

		std::array<SFrameBuffer, 2> mBlurs;

		Shader mShader;
		Shader mBlurShader;
		Shader mAOShader;
		Shader mAOBlurShader;
		Shader mSkyboxShader;

		Skybox mSkybox;

		STexture mWood;

		buffers::SDataBuffer mFrameInfoBuffer;
		buffers::SDataBuffer mLightInfoBuffer;
		buffers::SDataBuffer mObjectInfoBuffer;
		buffers::SDataBuffer mMaterialInfoBuffer;
		buffers::SDataBuffer mMorphInfoBuffer;

		//Shadowmap2 mShadowmap;
		Shader mShadowShader;

		ScreenQuad mScreenQuad;
	};

}
