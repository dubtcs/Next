#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/DataBuffer.h>

namespace nxt
{

	struct NXT_API AnimationTrack
	{
		int32_t target; // index of mesh target;
		nxtEasingStyle interpolation;
		Shared<buffers::DataBuffer> inputBuffer;
		Shared<buffers::DataBuffer> outputBuffer;
	};

	struct NXT_API Animation
	{
		std::vector<AnimationTrack> tracks;
	};

}
