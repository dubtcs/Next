#pragma once

#include "Texture.h"

namespace nxt
{

	class NXT_API TextureMS
	{
	public:
		TextureMS(int32_t width, int32_t height, uint32_t samples, nxtTextureFormat textureFormat = nxtTextureFormat_SRGB);
	protected:
		uint32_t mID;
		uint32_t mWidth;
		uint32_t mHeight;
		uint32_t mSamples;
		nxtTextureFormat mFormat;
		nxtTextureFormatInternal mInternalFormat;
	};

	using STextureMS = Shared<TextureMS>;

}
