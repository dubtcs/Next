#pragma once

#include <nxt/EngineCore.h>

#include "BufferEnums.h"

#include <nxt/core/GenericEnums.h>

namespace nxt::buffers
{

	/*
	Could use new naming?
	vertex_buffer as type name = Shared<VertexBuffer>

	*/

	class NXT_API VertexBuffer
	{
	public:
		static Shared<VertexBuffer> Create(size_t byteSize, void* data = nullptr, BUFFER_USAGE_ bufferUsage = BUFFER_USAGE_STATIC);
		void Draw(DRAW_MODE_ mode = DRAW_MODE_TRIANGLES) const;
		bool Bind() const;
		bool Unbind() const;
		void SetBufferSubData(size_t byteSize, size_t byteOffset, void* data) const;
		void SetLayoutPosition(uint32_t position, uint32_t amount, DATA_TYPE_ dataType, uint32_t byteStride = 0, uint32_t byteOffset = 0, bool normalized = false);
		VertexBuffer(size_t byteSize, void* data = nullptr, BUFFER_USAGE_ bufferUsage = BUFFER_USAGE_STATIC);
		~VertexBuffer();
	protected:
		uint32_t mID;
		uint32_t mVertexArrayId; // GL SPECIFIC >:(
	};

	using vertex_buffer = Shared<VertexBuffer>;

}
