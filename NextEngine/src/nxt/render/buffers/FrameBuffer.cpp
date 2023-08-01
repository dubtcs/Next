
#include "FrameBuffer.h"

#include <nxt/core/log/Log.h>

#include <glad/glad.h>

namespace nxt
{

	FrameBuffer::FrameBuffer(const SFrameTexture& color) :
		mWidth{ color->mWidth },
		mHeight{ color->mHeight }
	{
		glCreateFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
		mColorTextures.push_back(color);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color->mTarget, color->mID, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			NXT_LOG_CRIT("FrameBuffer creation failure. Code {0}", (std::stringstream{} << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER)).str());
		}
	}

	FrameBuffer::FrameBuffer(int32_t width, int32_t height, uint32_t samples) :
		mWidth{ width },
		mHeight{ height }
	{
		glCreateFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
		SFrameTexture color{ NewShared<FrameTexture>(width, height, samples, nxtTextureFormat_RGB, nxtTextureFormatInternal_RGB8) };
		SFrameTexture depth{ NewShared<FrameTexture>(width, height, samples, nxtTextureFormat_DepthStencil, nxtTextureFormatInternal_Depth24Stencil8) };
		AttachTexture(color, nxtTextureAttachment_Color0);
		AttachTexture(depth, nxtTextureAttachment_Depth);
		uint32_t s{ glCheckFramebufferStatus(GL_FRAMEBUFFER) };
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			NXT_LOG_CRIT("FrameBuffer creation failure. Code {0}", (std::stringstream{} << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER)).str());
		}
	}

	void FrameBuffer::AttachTexture(const SFrameTexture& tex, nxt_enum attachment)
	{
		if (attachment >= nxtTextureAttachment_Color0 && attachment < (nxtTextureAttachment_Color0 + 31))
		{
			mColorTextures.push_back(tex);
		}
		else if (attachment == nxtTextureAttachment_Depth)
		{
			mDepth = tex;
		}
		else
		{
			NXT_LOG_WARN("Attempting to attach texture to out of bounds attachment.");
			return;
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex->mTarget, tex->mID, 0);
		return;
	}

	void FrameBuffer::PushToViewport() const
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mWidth, mHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const SFrameTexture& FrameBuffer::GetTexture(uint32_t index) const
	{
		return mColorTextures[index];
	}

}
