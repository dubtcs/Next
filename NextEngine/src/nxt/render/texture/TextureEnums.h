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
		nxtTextureTarget_Cubemap = 0x8513,
		nxtTextureTarget_CubemapX = 0x8515,
		nxtTextureTarget_2DMS = 0x9100
	};

	enum nxtTextureFormat : nxt_enum
	{
		nxtTextureFormat_R = 0x1903,
		nxtTextureFormat_RG = 0x8227,
		nxtTextureFormat_RGB = 0x1907,
		nxtTextureFormat_RGBA = 0x1908,
		nxtTextureFormat_DepthStencil = 0x84F9,
		nxtTextureFormat_SRGB = 0x8C40,
		nxtTextureFormat_SRGBA = 0x8C42,

		nxtTextureFormat_RGBF = nxtTextureFormat_RGB,
		nxtTextureFormat_RGBAF = nxtTextureFormat_RGBA
	};

	enum nxtTextureFormatInternal : nxt_enum
	{
		nxtTextureFormatInternal_R8 = 0x8229,
		nxtTextureFormatInternal_R16 = 0x822A,
		nxtTextureFormatInternal_R16F = 0x822D,
		nxtTextureFormatInternal_RG8 = 0x822B,
		nxtTextureFormatInternal_RGB8 = 0x8051,
		nxtTextureFormatInternal_RGBA8 = 0x8058,
		nxtTextureFormatInternal_RGBA32F = 0x8814,
		nxtTextureFormatInternal_RGB32F = 0x8815,
		nxtTextureFormatInternal_RGBA16F = 0x881A,
		nxtTextureFormatInternal_RGB16F = 0x881B,
		nxtTextureFormatInternal_Depth24Stencil8 = 0x88F0,
		nxtTextureFormatInternal_SRGB8 = 0x8C41,
		nxtTextureFormatInternal_SRGBA8 = 0x8C43,
	};

	enum nxtTextureParamName : nxt_enum
	{
		nxtTextureParamName_MaximizeFilter = 0x2800,
		nxtTextureParamName_MinimizeFilter = 0x2801,
		nxtTextureParamName_WrapS = 0x2802,
		nxtTextureParamName_WrapT = 0x2803
	};

	enum nxtTextureParam : nxt_enum
	{
		nxtTextureParam_ = 0,
		nxtTextureParam_Nearest = 0x2600,
		nxtTextureParam_Linear = 0x2601,
		nxtTextureParam_MipNearNear = 0x2700,
		nxtTextureParam_MipLinearNear = 0x2701,
		nxtTextureParam_MipNearLinear = 0x2702,
		nxtTextureParam_MipLinearLinear = 0x2703,
		nxtTextureParam_Repeat = 0x2901,
		nxtTextureParam_ClampToEdge = 0x812F
	};

	enum nxtTextureAttachment : nxt_enum
	{
		nxtTextureAttachment_Color0 = 0x8CE0,
		nxtTextureAttachment_Depth = 0x8D00,
		nxtTextureAttachment_Stencil = 0x8D20
	};

}
