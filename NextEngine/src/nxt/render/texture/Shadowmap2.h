#pragma once

#include <nxt/EngineCore.h>

namespace nxt
{

	class NXT_API Shadowmap2
	{
	public:
		Shadowmap2(const uint32_t& resolution = 1024);
		~Shadowmap2();
		void BeginRenderPass();
		void EndRenderPass(int32_t width, int32_t height);
		void BindTextureMap(uint32_t textureUnit = 0);
	protected:
		uint32_t mBufferID;
		uint32_t mTextureID;
		uint32_t mResolution;
	};

}
