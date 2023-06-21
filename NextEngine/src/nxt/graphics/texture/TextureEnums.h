#pragma once

#include <nxt/EngineCore.h>

namespace nxt::texture
{

	enum TEXTURETYPE_ : uint32_t
	{
		TEXTURETYPE_1D = 0x0DE0,
		TEXTURETYPE_2D = 0x0DE1,
		TEXTURETYPE_3D = 0x806F
	};

	enum TEXTUREFORMAT_ : uint32_t
	{
		TEXTUREFORMAT_RGB = 0x1907,
		TEXTUREFORMAT_RGBA = 0x1908
	};

	enum TEXTUREFORMAT_RAW_ : uint32_t
	{
		TEXTUREFORMAT_RAW_RGB8 = 0x8051,
		TEXTUREFORMAT_RAW_RGBA8 = 0x8058,
	};

}
