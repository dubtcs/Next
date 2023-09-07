#pragma once

#include <nxt/EngineCore.h>

#include <nxt/render/buffers/ArrayObject.h>
#include <nxt/render/buffers/DataBuffer.h>

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

	struct NXT_API Primitive
	{
		uint32_t count;
		int32_t material{ -1 };
		size_t byteOffset;
		nxtDrawMode mode;
		nxtDataType componentType;
		bool hasTangents{ false };
		bool hasIndices{ false };
		std::vector<BufferDataModifier> modifiers{};
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
		std::vector<Primitive> primitives;
		std::vector<int32_t> children;
		glm::mat4 matrix{ 1.f };
	};

}
