
#include "Texture.h"

#include <nxt/core/GenericEnums.h>
#include <nxt/core/log/Log.h>

#include <glad/glad.h>
#include <stb_image.h>

namespace nxt::texture
{

	Shared<Texture> Texture::Create(const std::filesystem::path& filepath)
	{
		return NewShared<Texture>(filepath);
	}

	Texture::Texture(const std::filesystem::path& filepath)
	{
		NXT_LOG_TRACE("Attempting Texture: \"{0}\"", filepath.string());
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

				glCreateTextures(GL_TEXTURE_2D, 1, &mID);
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

	Texture::~Texture()
	{
		glDeleteTextures(1, &mID);
	}

	void Texture::Bind() const
	{
		// Kept accidentally putting GL_TEXTURE0 here. First param is the unit index
		// GL_TEXTURE0 is 0x8... so it was way off
		glBindTextureUnit(0, mID);
	}

}
