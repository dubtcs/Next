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
		void CopyBufferData(Shared<DataBuffer> otherBuffer, size_t readOffset, size_t writeOffset, size_t byteSize);
		const nxtBufferTarget& GetTarget() const;
		void BindIndexed(int32_t index) const;
		void Bind() const;
		void Unbind() const;
	public:
		const size_t mByteSize;
	protected:
		const nxtBufferTarget mTarget;
		uint32_t mID;
	};

	// Shared Data Buffer
	using SDataBuffer = Shared<DataBuffer>;

}
