#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/texture/FrameTexture.h>

#include <vector>
#include <map>

namespace nxt
{

	class NXT_API FrameBuffer
	{
	public:
		FrameBuffer(const SFrameTexture& color);
		FrameBuffer(int32_t width, int32_t height, uint32_t samples);
		~FrameBuffer();
		void AttachTexture(const SFrameTexture& text, nxt_enum attachment);
		void DetachTexture(nxt_enum attachment);
		int32_t GetAttachmentCount() const;
		void PushToViewport() const;
		void Bind() const;
		const SFrameTexture& GetTexture(uint32_t index) const;
		const SFrameTexture& GetDepthTexture() const;
		static void Unbind();
	protected:
		uint32_t mID;
		int32_t mWidth;
		int32_t mHeight;
		SFrameTexture mDepth;
		std::vector<nxt_enum> mDrawBuffers;
		std::map<nxt_enum, SFrameTexture> mAttachmentsToTextures;
	};
	using SFrameBuffer = Shared<FrameBuffer>;

}
