#pragma once

#include <nxt/EngineCore.h>

#include "BufferEnums.h"

#include <nxt/render/texture/Texture.h>
#include <nxt/render/texture/FrameTexture.h>

namespace nxt::buffers
{

	class NXT_API FrameBuffer
	{
	public:
		static Shared<FrameBuffer> Create(uint32_t width, uint32_t height, uint32_t samples = 1);
		FrameBuffer(uint32_t width, uint32_t height, uint32_t samples = 1);
		~FrameBuffer();
	public:
		void PushData() const;
		void Bind() const;
		static void Unbind();
	protected:
		uint32_t mID;
		uint32_t mSamples;
		uint32_t mWidth;
		uint32_t mHeight;
		SFrameTexture mColorTexture;
		SFrameTexture mDepthTexture;
	};

	using SFrameBuffer = Shared<FrameBuffer>;

}
