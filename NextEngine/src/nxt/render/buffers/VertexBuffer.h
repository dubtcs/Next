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
		static Shared<VertexBuffer> Create(size_t byteSize, BUFFER_USAGE_ usage = BUFFER_USAGE_STATIC, void* data = nullptr);
		void Draw(DRAW_MODE_ mode = DRAW_MODE_TRIANGLES) const;
		bool Bind() const;
		bool Unbind() const;
		void SetLayoutPosition(uint32_t position, uint32_t amount, DATA_TYPE_ dataType, uint32_t byteStride = 0, uint32_t byteOffset = 0, bool normalized = false);
		VertexBuffer(size_t byteSize, BUFFER_USAGE_ usage = BUFFER_USAGE_STATIC, void* data = nullptr);
		~VertexBuffer();
	protected:
		uint32_t mID;
		uint32_t mVertexArrayId; // GL SPECIFIC >:(
	};

}
