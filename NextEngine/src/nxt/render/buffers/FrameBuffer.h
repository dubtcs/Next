#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/texture/FrameTexture.h>

#include <vector>

namespace nxt
{

	class NXT_API FrameBuffer
	{
	public:
		FrameBuffer(const SFrameTexture& color);
		FrameBuffer(int32_t width, int32_t height, uint32_t samples);
		void AttachTexture(const SFrameTexture& text, nxt_enum attachment);
		void PushToViewport() const;
		void Bind() const;
		static void Unbind();
		const SFrameTexture& GetTexture(uint32_t index) const;
	protected:
		uint32_t mID;
		int32_t mWidth;
		int32_t mHeight;
		std::vector<SFrameTexture> mColorTextures;
		std::vector<nxt_enum> mDrawBuffers;
		SFrameTexture mDepth;
		//SFrameTexture2 mStencil; // could use a map and enums for textures and targets??
	};
	using SFrameBuffer = Shared<FrameBuffer>;

}
