#pragma once

#include <nxt/EngineCore.h>

#include "BufferEnums.h"
#include "VertexBuffer.h"

namespace nxt::buffers
{

	class NXT_API ElementBuffer
	{
	public:
		static Shared<ElementBuffer> Create(size_t byteSize, void* indices, nxtBufferUsage usage = nxtBufferUsage_Static);
		ElementBuffer(size_t byteSize, void* indices, nxtBufferUsage usage = nxtBufferUsage_Static);
		~ElementBuffer();
		void Bind() const;
		void Draw(nxtDrawMode mode, uint32_t count, uint32_t offset = 0) const;
		bool AddVertexBuffer(Shared<VertexBuffer>& buffer);
	protected:
		uint32_t mID{ 0 };
		Shared<VertexBuffer> mVertexBuffer{ nullptr };
	};

	using SElementBuffer = Shared<ElementBuffer>;

}
