
#include "FrameTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

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

	FrameTexture::FrameTexture(const std::filesystem::path& filepath, nxtTextureTarget target) :
		mTarget{ target },
		mSamples{ 0 }
	{
		if (filepath.has_extension())
		{
			int32_t channels{ 0 };
			stbi_uc* data{ stbi_load(filepath.string().c_str(), &mWidth, &mHeight, &channels, 0) };
			if (data != nullptr)
			{
				nxtTextureFormat baseFormat{ nxtTextureFormat_RGB };
				nxtTextureFormatInternal internalFormat{ nxtTextureFormatInternal_RGB8 };
				if (channels == 4)
				{
					baseFormat = nxtTextureFormat_RGBA;
					internalFormat = nxtTextureFormatInternal_RGBA8;
				}
				mFormat = baseFormat;
				mInternalFormat = internalFormat;
				glCreateTextures(mTarget, 1, &mID);
				glTextureStorage2D(mID, 1, mInternalFormat, mWidth, mHeight);
				glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, mFormat, nxtDataType_UByte, data);
			}
			stbi_image_free(data);
		}
		else
		{
			NXT_LOG_CRIT("Filepath [{0}] is invalid.", filepath.string());
		}
	}

	void FrameTexture::SetData(nxtTextureFormat format, nxtDataType dataType, void* ptr)
	{
		glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, format, dataType, ptr);
	}

	void FrameTexture::SetParameter(nxtTextureParamName name, nxtTextureParam value)
	{
		glTextureParameteri(mID, name, value);
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
