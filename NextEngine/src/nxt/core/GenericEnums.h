#pragma once

#include <memory>

namespace nxt
{

	using nxt_enum = uint32_t;
	using nxt_uint = uint32_t;
	using nxt_int = int32_t;

	enum nxtDataType : nxt_enum
	{
		nxtDataType_Byte = 0x1400, // Byte
		nxtDataType_UByte = 0x1401, // Unsigned Byte
		nxtDataType_Short = 0x1402, // Short
		nxtDataType_UShort = 0x1403, // Unsigned Short
		nxtDataType_Int = 0x1404, // Integer
		nxtDataType_UInt = 0x1405, // Unsigned Integer
		nxtDataType_Float = 0x1406, // Float
	};

}

