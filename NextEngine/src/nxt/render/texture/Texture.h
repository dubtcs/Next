#pragma once

#include <nxt/EngineCore.h>

#include "TextureEnums.h"
#include <filesystem>

namespace nxt
{

	// This is in namespace texture because I'm not sure if I'm going to have separate 2D/3D texture classes

	class NXT_API Texture
	{
	public:
		static Shared<Texture> Create(const std::filesystem::path& filepath, TEXTURE_TARGET_ textureTarget = TEXTURE_TARGET_2D);
		static Shared<Texture> Create(int32_t width, int32_t height, TEXTURE_FORMAT_ textureFormat = TEXTURE_FORMAT_RGB, TEXTURE_TARGET_ target = TEXTURE_TARGET_2D);
		void SetData(TEXTURE_FORMAT_ format, DATA_TYPE_ dataType, void* data);
		void Bind(uint32_t textureUnit = 0) const;
		Texture(const std::filesystem::path& filepath, TEXTURE_TARGET_ textureTarget = TEXTURE_TARGET_2D);
		Texture(int32_t width, int32_t height, TEXTURE_FORMAT_ textureFormat = TEXTURE_FORMAT_RGB, TEXTURE_TARGET_ target = TEXTURE_TARGET_2D);
		~Texture();
	protected:
		void SetInternalFormat();
	protected:
		uint32_t mID{ 0 };
		int32_t mWidth;
		int32_t mHeight;
		const TEXTURE_TARGET_ mTarget;
		TEXTURE_FORMAT_ mFormat;
		TEXTURE_FORMAT_INTERNAL_ mInternalFormat;
	};

}
