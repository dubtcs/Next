#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt::texture
{

	enum TEXTURETYPE_ : nxt_enum
	{
		TEXTURE_TYPE_1D = 0x0DE0,
		TEXTURE_TYPE_2D = 0x0DE1,
		TEXTURE_TYPE_3D = 0x806F
	};

	enum TEXTURE_FORMAT_ : nxt_enum
	{
		TEXTURE_FORMAT_RGB = 0x1907,
		TEXTURE_FORMAT_RGBA = 0x1908
	};

	enum TEXTURE_FORMAT_INTERNAL_ : nxt_enum
	{
		TEXTURE_FORMAT_INTERNAL_RGB8 = 0x8051,
		TEXTURE_FORMAT_INTERNAL_RGBA8 = 0x8058,
	};

}
