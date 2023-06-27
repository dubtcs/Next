#pragma once

#include <nxt/EngineCore.h>

#include "BufferEnums.h"
#include "VertexBuffer.h"

namespace nxt::buffers
{

	class NXT_API ElementBuffer
	{
	public:
		static Shared<ElementBuffer> Create(size_t byteSize, void* indices, BUFFER_USAGE_ usage = BUFFER_USAGE_STATIC);
		ElementBuffer(size_t byteSize, void* indices, BUFFER_USAGE_ usage = BUFFER_USAGE_STATIC);
		~ElementBuffer();
		void Bind() const;
		void Draw(DRAW_MODE_ mode, uint32_t count, uint32_t offset = 0) const;
		bool AddVertexBuffer(Shared<VertexBuffer>& buffer);
	protected:
		uint32_t mID{ 0 };
		Shared<VertexBuffer> mVertexBuffer{ nullptr };
	};

}
