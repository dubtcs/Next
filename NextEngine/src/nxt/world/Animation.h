#pragma once

#include <nxt/EngineCore.h>

#include "Primitive.h"
#include "AnimationEnums.h"

#include <nxt/render/buffers/DataBuffer.h>

namespace nxt
{

	struct NXT_API AnimationTrack
	{
		int32_t target; // index of mesh target;
		int32_t indicesPerElement; // amount of indices to form one element of data. eg 3 == vec3, 4 == vec4
		nxtAnimationTarget animationTarget;
		nxtInterpolation interpolation;
		std::vector<float> timing; // keyframe timings
		std::vector<float> data; // data format enum controls interpretation as vec3 or whatever
	};

	struct NXT_API Animation
	{
		float totalRuntime{ 0.f };
		std::vector<AnimationTrack> tracks;
	};

	/*
	Rework:
	Each animation will only have one channel per mesh.
	Usually only one sampler per channel.
	*/

}
