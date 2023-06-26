#pragma once

#include <nxt/EngineCore.h>

#include "TextureEnums.h"
#include <filesystem>

namespace nxt::texture
{

	// This is in namespace texture because I'm not sure if I'm going to have separate 2D/3D texture classes

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
