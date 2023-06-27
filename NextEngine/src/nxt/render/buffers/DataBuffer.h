#pragma once

#include <nxt/EngineCore.h>
#include "BufferEnums.h"

namespace nxt::buffers
{

	class NXT_API DataBuffer
	{
	public:
		static Shared<DataBuffer> Create(size_t byteSize, void* data = nullptr, 
			BUFFER_TARGET_ bufferTarget = BUFFER_TARGET_ARRAY_BUFFER, BUFFER_USAGE_ bufferUsage = BUFFER_USAGE_STATIC);
		DataBuffer(size_t byteSize, void* data = nullptr,
			BUFFER_TARGET_ bufferTarget = BUFFER_TARGET_ARRAY_BUFFER, BUFFER_USAGE_ bufferUsage = BUFFER_USAGE_STATIC);
		~DataBuffer();
	public:
		void SetSubData(size_t byteSize, size_t byteOffset, void* data);
		const BUFFER_TARGET_& GetTarget() const;
		void Bind() const;
		void Unbind() const;

		// PLACEHOLDER
		void Draw(DRAW_MODE_ drawMode, uint32_t count, uint32_t byteOffset, DATA_TYPE_ datType = DATA_TYPE_FLOAT) const;
	protected:
		const BUFFER_TARGET_ mTarget;
		uint32_t mID;
	};

}
