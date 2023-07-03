#pragma once

#include <nxt/EngineCore.h>

#include "BufferEnums.h"

namespace nxt::buffers
{

	/*
	Array Objects must be unbound before deleting any associated buffers
	*/

	class NXT_API ArrayObject
	{
	public:
		static Shared<ArrayObject> Create();
		ArrayObject();
		~ArrayObject();
	public:
		void SetLayoutPosition(uint32_t position, uint32_t amount, nxtDataType dataType, uint32_t byteStride = 0, uint32_t byteOffset = 0, bool normalized = false);
		void Bind() const;
		static void Unbind();
	protected:
		uint32_t mID;
	};

}
