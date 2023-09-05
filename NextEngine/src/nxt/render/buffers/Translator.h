#pragma once

#include <nxt/EngineCore.h>

#include <nxt/core/GenericEnums.h>

namespace nxt
{

	struct NXT_API BufferTranslator
	{
		nxtDataType dataType; // uint32
		int32_t count;
		size_t byteOffset;
	};

}
