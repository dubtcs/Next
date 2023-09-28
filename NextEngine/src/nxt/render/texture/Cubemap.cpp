
#include "Cubemap.h"

#include <nxt/core/log/Log.h>

#include <stb_image.h>
#include <glad/glad.h>

namespace nxt
{

	// Can probably use a STexture for the parent of all sub textures?

	Cubemap::Cubemap(const std::initializer_list<std::filesystem::path>& textureLocations)
	{
		if (textureLocations.size() > 0)
		{
			if (textureLocations.size() < 6)
			{
				NXT_LOG_WARN("Less than 6 textures supplied for cubemap.");
			}

			glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, mID);

			int32_t width{};
			int32_t height{};
			int32_t channels{};
			stbi_uc* data{};

			int32_t i{ 0 };
			for (const std::filesystem::path& curPath : textureLocations)
			{
				if (curPath.has_extension())
				{
					data = stbi_load(curPath.string().c_str(), &width, &height, &channels, 0);
					if (data != nullptr)
					{
						nxtTextureFormat baseFormat{ nxtTextureFormat_RGB };
						nxtTextureFormatInternal internalFormat{ nxtTextureFormatInternal_RGB8 };

						if (channels == 4)
						{
							baseFormat = nxtTextureFormat_RGBA;
							internalFormat = nxtTextureFormatInternal_RGBA8;
						}

						NXT_LOG_INFO("Channels  {0}", channels);
						NXT_LOG_TRACE("Dimensions: {0}, {1}", width, height);

						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, baseFormat, nxtDataType_UByte, data);
					}
					stbi_image_free(data);
				}
				else
				{
					NXT_LOG_CRIT("No filename for: {0}", curPath.string());
				}
				i++;
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		}
		else
		{
			NXT_LOG_CRIT("No textures given for cubemap.");
		}
	}

	void Cubemap::Bind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
	}

	void Cubemap::Unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

}
