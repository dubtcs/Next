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
		static Shared<FrameBuffer> Create(const SFrameTexture& colorTexture, const SFrameTexture& depthTexture);
		static Shared<FrameBuffer> Create(int32_t width, int32_t height, uint32_t samples = 1);
		FrameBuffer(SFrameTexture colorTexture, SFrameTexture depthTexture);
		FrameBuffer(int32_t width, int32_t height, uint32_t samples = 1);
		~FrameBuffer();
	public:
		void PushToViewport() const;
		void Bind() const;
		void BindTexture(uint32_t unit) const;
		static void Unbind();
	protected:
		uint32_t mID;
		uint32_t mSamples;
		int32_t mWidth;
		int32_t mHeight;
		SFrameTexture mColorTexture;
		SFrameTexture mDepthTexture;
	};

	using SFrameBuffer = Shared<FrameBuffer>;

}
