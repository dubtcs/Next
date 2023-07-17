#pragma once

#include <nxt/EngineCore.h>

#include <array>

namespace nxt
{
	// TEMP
	// use framebuffer classes later

	class NXT_API PointShadowmap
	{
	public:
		PointShadowmap();
		~PointShadowmap();
		void BindDepth() const;
		void Bind() const;
		static void UnbindDepth();
		static void Unbind();
	protected:
		uint32_t mID;
		uint32_t mBuffer;
		std::array<uint32_t, 6> mSubtextures;
	};

	using SPointShadowmap = Shared<PointShadowmap>;

}
