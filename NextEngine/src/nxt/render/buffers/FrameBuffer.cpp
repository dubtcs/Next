
#include "FrameBuffer.h"

#include <glad/glad.h>

namespace nxt::buffers
{

	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) :
		mColorTexture{ Texture::Create(width, height) },
		mDepthTexture{ Texture::Create(width, height, nxtTextureFormat_DepthStencil) }
	{
		glGenFramebuffers(1, &mID);
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mColorTexture->mTarget, mColorTexture->mID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthTexture->mTarget, mDepthTexture->mID, 0);
	}

	FrameBuffer::~FrameBuffer()
	{
		glDeleteFramebuffers(1, &mID);
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mID);
	}

	void FrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
