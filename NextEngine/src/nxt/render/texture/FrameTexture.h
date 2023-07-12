#pragma once

#include <nxt/EngineCore.h>

#include "TextureEnums.h"

namespace nxt
{

	namespace buffers
	{
		class FrameBuffer;
	}

	class NXT_API FrameTexture
	{
	public:
		static Shared<FrameTexture> Create(uint32_t width, uint32_t height, uint32_t samples, nxtTextureFormat textureFormat = nxtTextureFormat_RGB, nxtTextureTarget target = nxtTextureTarget_2DMS);
		FrameTexture(uint32_t width, uint32_t height, uint32_t samples, nxtTextureFormat textureFormat = nxtTextureFormat_RGB, nxtTextureTarget target = nxtTextureTarget_2DMS);
		~FrameTexture();
		void Bind() const;
		void Unbind() const;
	protected:
		uint32_t mID;
		nxtTextureTarget mTarget;
		nxtTextureFormatInternal mInternalFormat;
		friend class buffers::FrameBuffer;
	};

	using SFrameTexture = Shared<FrameTexture>;

}
