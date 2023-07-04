#pragma once

#include <nxt/EngineCore.h>
#include "BufferEnums.h"

namespace nxt::buffers
{

	class NXT_API DataBuffer
	{
	public:
		static Shared<DataBuffer> Create(size_t byteSize, void* data = nullptr, 
			nxtBufferTarget bufferTarget = nxtBufferTarget_ArrayBuffer, nxtBufferUsage bufferUsage = nxtBufferUsage_Static);
		DataBuffer(size_t byteSize, void* data = nullptr,
			nxtBufferTarget bufferTarget = nxtBufferTarget_ArrayBuffer, nxtBufferUsage bufferUsage = nxtBufferUsage_Static);
		~DataBuffer();
	public:
		void SetSubData(size_t byteSize, size_t byteOffset, void* data);
		const nxtBufferTarget& GetTarget() const;
		void Bind() const;
		void Unbind() const;

		// PLACEHOLDER
		void Draw(nxtDrawMode drawMode, uint32_t count, uint32_t byteOffset, nxtDataType datType = nxtDataType_Float) const;
	protected:
		const nxtBufferTarget mTarget;
		uint32_t mID;
	};

	// Shared Data Buffer
	using SDataBuffer = Shared<DataBuffer>;

}
