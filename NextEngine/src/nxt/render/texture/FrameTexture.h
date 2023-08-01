#pragma once

#include <nxt/EngineCore.h>
#include "TextureEnums.h"

namespace nxt
{

	struct TextureParams
	{
		nxtTextureParam MinimizeFilter{ nxtTextureParam_Linear };
		nxtTextureParam MagnifyFilter{ nxtTextureParam_Linear };
		nxtTextureParam WrapFilterX{ nxtTextureParam_ClampToEdge };
		nxtTextureParam WrapFilterY{ nxtTextureParam_ClampToEdge };
	};

	class NXT_API FrameTexture
	{
	public:
		FrameTexture(int32_t width, int32_t height, uint32_t samples, nxtTextureFormat format, nxtTextureFormatInternal internalFormat, TextureParams params = {});
		~FrameTexture();
		void BindToTarget(nxtTextureTarget target) const;
		void BindToUnit(uint32_t unit) const;
	protected:
		uint32_t mID;
		int32_t mWidth;
		int32_t mHeight;
		uint32_t mSamples;
		nxtTextureTarget mTarget;
		nxtTextureFormat mFormat;
		nxtTextureFormatInternal mInternalFormat;
	public:
		friend class FrameBuffer;
	};
	using SFrameTexture = Shared<FrameTexture>;

}
