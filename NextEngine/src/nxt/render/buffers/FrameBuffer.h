#pragma once

#include <nxt/EngineCore.h>

#include "BufferEnums.h"

#include <nxt/render/texture/Texture.h>

namespace nxt::buffers
{

	class NXT_API FrameBuffer
	{
	public:
		FrameBuffer(uint32_t width, uint32_t height);
		~FrameBuffer();
	public:
		void Bind() const;
		static void Unbind();
	protected:
		uint32_t mID;
		STexture mColorTexture;
		STexture mDepthTexture;
	};

	using SFrameBuffer = Shared<FrameBuffer>;

}
