
#include "FrameTexture.h"

#include <glad/glad.h>

#include "Texture.h"

namespace nxt
{

	FrameTexture::FrameTexture(uint32_t width, uint32_t height, uint32_t samples, nxtTextureFormat textureFormat, nxtTextureTarget target) :
		mTarget{ target }
	{
		Texture::SetInternalFormat(textureFormat, &mInternalFormat);
		glCreateTextures(target, 1, &mID);
		if (samples > 1)
		{
			glTextureStorage2DMultisample(mID, samples, mInternalFormat, width, height, true);
		}
		else
		{
			glTextureStorage2D(mID, 1, mInternalFormat, width, height);
		}
		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	FrameTexture::~FrameTexture()
	{
		glDeleteTextures(1, &mID);
	}

	void FrameTexture::Bind() const
	{
		glBindTexture(mTarget, mID);
	}

	void FrameTexture::Unbind() const
	{
		glBindTexture(mTarget, 0);
	}

	Shared<FrameTexture> FrameTexture::Create(uint32_t width, uint32_t height, uint32_t samples, nxtTextureFormat textureFormat, nxtTextureTarget target)
	{
		return NewShared<FrameTexture>(width, height, samples, textureFormat, target);
	}

}
