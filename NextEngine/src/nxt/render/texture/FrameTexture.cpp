
#include "FrameTexture.h"

#include <glad/glad.h>

namespace nxt
{

	FrameTexture::FrameTexture(int32_t width, int32_t height, uint32_t samples, nxtTextureFormat format, nxtTextureFormatInternal internalFormat, TextureParams params) :
		mWidth{ width },
		mHeight{ height },
		mSamples{ samples },
		mFormat{ format },
		mInternalFormat{ internalFormat }
	{
		mTarget = (mSamples > 1) ? nxtTextureTarget_2DMS : nxtTextureTarget_2D;
		glCreateTextures(mTarget, 1, &mID);
		if (mSamples > 1)
		{
			glTextureStorage2DMultisample(mID, mSamples, mInternalFormat, mWidth, mHeight, true); // Allow final param as adjustable?
		}
		else
		{
			glTextureStorage2D(mID, 1, mInternalFormat, mWidth, mHeight);
		}
		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, params.MinimizeFilter);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, params.MagnifyFilter);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_S, params.WrapFilterX);
		glTextureParameteri(mID, GL_TEXTURE_WRAP_T, params.WrapFilterY);
	}

	FrameTexture::~FrameTexture()
	{
		glDeleteTextures(1, &mID);
	}

	void FrameTexture::BindToTarget(nxtTextureTarget target) const
	{
		glBindTexture(target, mID);
	}

	void FrameTexture::BindToUnit(uint32_t unit) const
	{
		glBindTextureUnit(unit, mID);
	}

}
