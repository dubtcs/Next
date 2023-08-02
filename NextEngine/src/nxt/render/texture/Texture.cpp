
#include "Texture.h"

#include <nxt/core/GenericEnums.h>
#include <nxt/core/log/Log.h>

#include <glad/glad.h>
#include <stb_image.h>

namespace nxt
{

	Shared<Texture> Texture::Create(const std::filesystem::path& filepath, nxtTextureTarget textureTarget)
	{
		return NewShared<Texture>(filepath, textureTarget);
	}

	Shared<Texture> Texture::Create(int32_t width, int32_t height, nxtTextureFormat textureFormat, nxtTextureTarget target)
	{
		return NewShared<Texture>(width, height, textureFormat, target);
	}

	Texture::Texture(int32_t width, int32_t height, nxtTextureFormat textureFormat, nxtTextureTarget target) :
		mWidth{ width },
		mHeight{ height },
		mTarget{ target },
		mFormat{ textureFormat }
	{
		SetInternalFormat(textureFormat, &mInternalFormat);
		glCreateTextures(target, 1, &mID);
		glTextureStorage2D(mID, 1, mInternalFormat, width, height);
		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	Texture::Texture(const std::filesystem::path& filepath, nxtTextureTarget textureTarget) :
		mTarget{ textureTarget }
	{
		NXT_LOG_TRACE("Creating texture: \"{0}\"", filepath.string());
		if (!filepath.empty())
		{
			int32_t width{ 0 };
			int32_t height{ 0 };
			int32_t channels{ 0 };

			stbi_uc* data{ stbi_load(filepath.string().c_str(), &width, &height, &channels, 0)};
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

				mWidth = width;
				mHeight = height;

				glCreateTextures(textureTarget, 1, &mID);
				glBindTexture(mTarget, mID);
				glTextureStorage2D(mID, 1, internalFormat, width, height);
				glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureSubImage2D(mID, 0, 0, 0, width, height, baseFormat, nxtDataType_UByte, data);

			}
			else
			{
				NXT_LOG_CRIT("Texture file not valid: \"{0}\"", filepath.string());
			}
			stbi_image_free(data);

		}
		else
		{
			NXT_LOG_CRIT("Filepath not valid: {0}", filepath.string());
		}
	}

	void Texture::SetInternalFormat(nxtTextureFormat from, nxtTextureFormatInternal* target)
	{
		switch (from)
		{
			case(nxtTextureFormat_R): { *target = nxtTextureFormatInternal_R8; return; }
			case(nxtTextureFormat_RG): { *target = nxtTextureFormatInternal_RG8; return; }
			case(nxtTextureFormat_RGB): { *target = nxtTextureFormatInternal_RGB8; return; }
			case(nxtTextureFormat_RGBA): { *target = nxtTextureFormatInternal_RGBA8; return; }
			case(nxtTextureFormat_DepthStencil): { *target = nxtTextureFormatInternal_Depth24Stencil8; return; }
			case(nxtTextureFormat_SRGB): { *target = nxtTextureFormatInternal_SRGB8; return; }
			case(nxtTextureFormat_SRGBA): { *target = nxtTextureFormatInternal_SRGBA8; return; }
			//case(nxtTextureFormat_RGBF): { *target = nxtTextureFormatInternal_RGB32F; return; } // Enforcing 32 bit for now.. Overkill
			//case(nxtTextureFormat_RGBAF): { *target = nxtTextureFormatInternal_RGBA32F; return; } // Enforcing 32 bit for now.. Overkill
		}
		NXT_LOG_CRIT("No internal format found for requested format");
	}

	void Texture::GenerateMipmaps() const
	{
		glGenerateTextureMipmap(mID);
	}

	void Texture::SetParameter(nxtTextureParamName name, nxtTextureParam param)
	{
		glTextureParameteri(mID, name, param);
	}

	void Texture::SetData(nxtTextureFormat format, nxtDataType dataType, void* data)
	{
		glTextureSubImage2D(mID, 0, 0, 0, mWidth, mHeight, format, dataType, data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &mID);
	}

	void Texture::Bind(uint32_t textureUnit) const
	{
		// Kept accidentally putting GL_TEXTURE0 here. First param is the unit index
		// GL_TEXTURE0 is 0x8... so it was way off
		glBindTextureUnit(textureUnit, mID);
	}

}
