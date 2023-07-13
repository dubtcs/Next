
#include "FrameBuffer.h"

#include <nxt/core/log/Log.h>
#include <glad/glad.h>

namespace nxt::buffers
{

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint32_t samples) :
		mWidth{ width },
		mHeight{ height }
	{
		glCreateFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);

		nxtTextureTarget target{ samples > 1 ? nxtTextureTarget_2DMS : nxtTextureTarget_2D };
		mColorTexture = FrameTexture::Create(width, height, samples, nxtTextureFormat_RGBA, target);
		mDepthTexture = FrameTexture::Create(width, height, samples, nxtTextureFormat_DepthStencil, target);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mColorTexture->mTarget, mColorTexture->mID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture->mTarget, mDepthTexture->mID, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			NXT_LOG_CRIT("FrameBuffer creation failure. Code {0}", (std::stringstream{} << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER)).str());
		}
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &mID);
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

	SFrameBuffer FrameBuffer::Create(uint32_t width, uint32_t height, uint32_t samples)
	{
		return NewShared<FrameBuffer>(width, height, samples);
	}

}
