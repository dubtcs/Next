#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/DataBuffer.h>

namespace nxt
{

	struct NXT_API KeyframeAccessor
	{
		int32_t count;
		int32_t type;
		nxtDataType dataType;
		Shared<buffers::DataBuffer> buffer;
	};

	struct NXT_API KeyframeSampler
	{
		KeyframeAccessor timing;
		KeyframeAccessor data;
		nxtEasingStyle interpolation;
	};

	struct NXT_API Animation
	{
		std::vector<KeyframeSampler> keyframes;
	};

}
