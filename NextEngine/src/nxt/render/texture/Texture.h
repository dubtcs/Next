#pragma once

#include <nxt/EngineCore.h>

#include "TextureEnums.h"
#include <filesystem>

namespace nxt
{

	namespace buffers
	{
		class FrameBuffer;
	}

	class NXT_API Texture
	{
		friend class buffers::FrameBuffer;
	public:
		static Shared<Texture> Create(const std::filesystem::path& filepath, nxtTextureTarget textureTarget = nxtTextureTarget_2D);
		static Shared<Texture> Create(int32_t width, int32_t height, nxtTextureFormat textureFormat = nxtTextureFormat_SRGB, nxtTextureTarget target = nxtTextureTarget_2D);
		Texture(const std::filesystem::path& filepath, nxtTextureTarget textureTarget = nxtTextureTarget_2D);
		Texture(int32_t width, int32_t height, nxtTextureFormat textureFormat = nxtTextureFormat_SRGB, nxtTextureTarget target = nxtTextureTarget_2D);
		~Texture();
		void SetParameter(nxtTextureParamName name, nxtTextureParam param);
		void SetData(nxtTextureFormat format, nxtDataType dataType, void* data);
		void GenerateMipmaps() const;
		void Bind(uint32_t textureUnit = 0) const;
		static void SetInternalFormat(nxtTextureFormat from, nxtTextureFormatInternal* target);
		std::string NAME{ "NOT SET" };
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
