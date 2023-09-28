#pragma once

#include <nxt/EngineCore.h>

#include "PrimitiveEnums.h"

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>
#include <nxt/core/clock/Clock.h>

#include <map>

namespace nxt
{

	struct NXT_API ModifierInfo
	{
		size_t byteOffset;
		size_t elementByteSize;
		size_t targetByteStride;
		size_t targetByteOffset;
		buffers::SDataBuffer dataBuffer;
	};

	struct NXT_API BufferDataModifier
	{
		buffers::SDataBuffer target;
		ModifierInfo info{};
		std::vector<uint16_t> indices{};
	};

	struct NXT_API Morph
	{
		using target_map = std::unordered_map<nxtMorphTarget, std::vector<glm::vec3>>;
		target_map targets;
	};

	struct NXT_API Primitive
	{
		uint32_t count;
		int32_t material{ -1 };
		size_t byteOffset;
		nxtDrawMode mode;
		nxtDataType componentType;
		nxt_enum features{ 0 };
		bool hasTangents{ false };
		bool hasIndices{ false };
		std::vector<BufferDataModifier> modifiers{};
		std::vector<Morph> morphTargets{};
		buffers::SDataBuffer buffer;
		buffers::SArrayObject arrayObject{ NewShared<buffers::ArrayObject>() };
	};

	struct NXT_API Mesh // also a node
	{
		std::vector<Primitive> primitives;
		std::vector<Mesh> children;
		glm::mat4 matrix{ 1.f };
	};

	struct NXT_API Mesh2
	{
		using AnimationID = int32_t;
		using TrackID = int32_t;
		using AnimationTrackMap = std::map<AnimationID, TrackID>;
		struct NXT_API MatrixComposition
		{
			glm::vec3 position{ 0.f };
			glm::vec4 rotation{ 0.f };
			glm::vec3 scale{ 1.f };
		};
		struct NXT_API AnimationInfo
		{
			int32_t currentKeyframe{ 0 };
		};
		MatrixComposition matrix;
		AnimationInfo animationInfo;
		std::vector<Primitive> primitives;
		std::vector<int32_t> children;
		std::vector<float> morphWeights;
		AnimationTrackMap animations;
	};

}
