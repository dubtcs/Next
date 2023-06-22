#pragma once

#include <nxt/EngineCore.h>

#include "TextureEnums.h"
#include <filesystem>

namespace nxt::texture
{

	class NXT_API Texture
	{
	public:
		static Shared<Texture> Create(const std::filesystem::path& filepath);
		void Bind(uint32_t textureUnit = 0) const;
		Texture(const std::filesystem::path& filepath);
		~Texture();
	protected:
		uint32_t mID{ 0 };
	};

}
