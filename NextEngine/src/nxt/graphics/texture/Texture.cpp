
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
		if (!filepath.empty())
		{
			int32_t width{ 0 };
			int32_t height{ 0 };
			int32_t channels{ 0 };

			stbi_uc* data{ stbi_load(filepath.string().c_str(), &width, &height, &channels, 0)};
			if (data != nullptr)
			{
				TEXTUREFORMAT_RAW_ glf{ TEXTUREFORMAT_RAW_RGB8 };
				TEXTUREFORMAT_ fmt{ TEXTUREFORMAT_RGB };

				if (channels == 4)
				{
					glf = TEXTUREFORMAT_RAW_RGBA8;
					fmt = TEXTUREFORMAT_RGBA;
				}

				glCreateTextures(GL_TEXTURE_2D, 1, &mID);
				glTextureStorage2D(mID, 1, glf, width, height);
				glTextureParameteri(mID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(mID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTextureSubImage2D(mID, 0, 0, 0, width, height, fmt, GL_UNSIGNED_BYTE, data);

				stbi_image_free(data);
			}
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
		glBindTextureUnit(GL_TEXTURE0, mID);
	}

}
