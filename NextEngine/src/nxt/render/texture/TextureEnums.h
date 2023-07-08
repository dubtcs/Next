#pragma once

#include <nxt/EngineCore.h>
#include <nxt/core/GenericEnums.h>

namespace nxt
{

	enum nxtTextureTarget : nxt_enum
	{
		nxtTextureTarget_1D = 0x0DE0,
		nxtTextureTarget_2D = 0x0DE1,
		nxtTextureTarget_3D = 0x806F,
		nxtTextureTarget_Cubemap = 0x8513
	};

	enum nxtTextureFormat : nxt_enum
	{
		nxtTextureFormat_R = 0x1903,
		nxtTextureFormat_RG = 0x8227,
		nxtTextureFormat_RGB = 0x1907,
		nxtTextureFormat_RGBA = 0x1908,
		nxtTextureFormat_DepthStencil = 0x84F9
	};

	enum nxtTextureFormatInternal : nxt_enum
	{
		nxtTextureFormatInternal_R8 = 0x8229,
		nxtTextureFormatInternal_RG8 = 0x822B,
		nxtTextureFormatInternal_RGB8 = 0x8051,
		nxtTextureFormatInternal_RGBA8 = 0x8058,
		nxtTextureFormatInternal_Depth24Stencil8 = 0x88F0
	};

}
