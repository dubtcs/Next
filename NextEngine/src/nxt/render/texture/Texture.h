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
		static Shared<Texture> Create(const std::filesystem::path& filepath, nxtTextureTarget textureTarget = nxtTextureTarget_2D);
		static Shared<Texture> Create(int32_t width, int32_t height, nxtTextureFormat textureFormat = nxtTextureFormat_RGB, nxtTextureTarget target = nxtTextureTarget_2D);
		void SetData(nxtTextureFormat format, nxtDataType dataType, void* data);
		void Bind(uint32_t textureUnit = 0) const;
		Texture(const std::filesystem::path& filepath, nxtTextureTarget textureTarget = nxtTextureTarget_2D);
		Texture(int32_t width, int32_t height, nxtTextureFormat textureFormat = nxtTextureFormat_RGB, nxtTextureTarget target = nxtTextureTarget_2D);
		~Texture();
	protected:
		void SetInternalFormat();
	protected:
		uint32_t mID{ 0 };
		int32_t mWidth;
		int32_t mHeight;
		const nxtTextureTarget mTarget;
		nxtTextureFormat mFormat;
		nxtTextureFormatInternal mInternalFormat;
	};

	// Shared Texture
	using STexture = Shared<Texture>;

}
