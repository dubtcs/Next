#pragma once

#include <nxt/EngineCore.h>

#include "Primitive.h"

#include <nxt/render/buffers/DataBuffer.h>

namespace nxt
{

	// Each model has a matrix
	// Use the animation keyframes to manipulate the matrix
	// No need for buffer manipulation

	struct NXT_API AnimationModifier
	{
		Shared<buffers::DataBuffer> timingBuffer;
		Shared<buffers::DataBuffer> dataBuffer;
	};

	struct NXT_API AnimationTrack
	{
		int32_t target; // index of mesh target;
		nxtInterpolation interpolation;
		
	};

	struct NXT_API Animation
	{
		std::vector<AnimationTrack> tracks;
	};

}
