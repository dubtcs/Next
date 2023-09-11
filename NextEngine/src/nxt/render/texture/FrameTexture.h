#pragma once

#include <nxt/EngineCore.h>
#include "TextureEnums.h"

#include <filesystem>

namespace nxt
{

	struct TextureParams
	{
		nxtTextureParam MinimizeFilter{ nxtTextureParam_Linear };
		nxtTextureParam MagnifyFilter{ nxtTextureParam_Linear };
		nxtTextureParam WrapFilterX{ nxtTextureParam_Repeat }; // previously clamp to edge
		nxtTextureParam WrapFilterY{ nxtTextureParam_Repeat }; // previously clamp to edge
	};

	// This CAN replace both TextureMultiSample and Texture

	class NXT_API FrameTexture
	{
	public:
		FrameTexture(int32_t width, int32_t height, uint32_t samples, nxtTextureFormat format, nxtTextureFormatInternal internalFormat, TextureParams params = {});
		~FrameTexture();
		void BindToUnit(uint32_t unit) const;
		void BindToTarget(nxtTextureTarget target) const;
		void SetParameter(nxtTextureParamName name, nxtTextureParam value);
		void SetData(nxtTextureFormat format, nxtDataType dataType, void* dataPointer);

		FrameTexture(const std::filesystem::path& filepath, nxtTextureTarget target = nxtTextureTarget_2D);
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
