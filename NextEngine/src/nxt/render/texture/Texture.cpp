
#include "Texture.h"

#include <nxt/core/GenericEnums.h>
#include <nxt/core/log/Log.h>

#include <glad/glad.h>
#include <stb_image.h>

namespace nxt
{

	Shared<Texture> Texture::Create(const std::filesystem::path& filepath, TEXTURE_TARGET_ textureTarget)
	{
		return NewShared<Texture>(filepath, textureTarget);
	}

	Shared<Texture> Texture::Create(int32_t width, int32_t height, TEXTURE_FORMAT_ textureFormat, TEXTURE_TARGET_ target)
	{
		return NewShared<Texture>(width, height, textureFormat, target);
	}

	Texture::Texture(int32_t width, int32_t height, TEXTURE_FORMAT_ textureFormat, TEXTURE_TARGET_ target) :
		mWidth{ width },
		mHeight{ height },
		mTarget{ target },
		mFormat{ textureFormat }
	{
		SetInternalFormat();
		glCreateTextures(target, 1, &mID);
		glTextureStorage2D(mID, 1, mInternalFormat, width, height);
		glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	Texture::Texture(const std::filesystem::path& filepath, TEXTURE_TARGET_ textureTarget) :
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
				TEXTURE_FORMAT_ baseFormat{ TEXTURE_FORMAT_RGB };
				TEXTURE_FORMAT_INTERNAL_ internalFormat{ TEXTURE_FORMAT_INTERNAL_RGB8 };

				if (channels == 4)
				{
					baseFormat = TEXTURE_FORMAT_RGBA;
					internalFormat = TEXTURE_FORMAT_INTERNAL_RGBA8;
				}

				mFormat = baseFormat;
				mInternalFormat = internalFormat;

				mWidth = width;
				mHeight = height;

				glCreateTextures(textureTarget, 1, &mID);
				glTextureStorage2D(mID, 1, internalFormat, width, height);
				glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureSubImage2D(mID, 0, 0, 0, width, height, baseFormat, DATA_TYPE_UBYTE, data);

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

	void Texture::SetInternalFormat()
	{
		switch (mFormat)
		{
			case(TEXTURE_FORMAT_R): {mInternalFormat = TEXTURE_FORMAT_INTERNAL_R8; return; }
			case(TEXTURE_FORMAT_RG): {mInternalFormat = TEXTURE_FORMAT_INTERNAL_RG8; return; }
			case(TEXTURE_FORMAT_RGB): {mInternalFormat = TEXTURE_FORMAT_INTERNAL_RGB8; return; }
			case(TEXTURE_FORMAT_RGBA): {mInternalFormat = TEXTURE_FORMAT_INTERNAL_RGBA8; return; }
		}
		NXT_LOG_CRIT("No internal format found for requested format");
	}

	void Texture::SetData(TEXTURE_FORMAT_ format, DATA_TYPE_ dataType, void* data)
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
