#pragma once

#include <memory>

namespace nxt
{

	using nxt_enum = uint32_t;
	using nxt_uint = uint32_t;
	using nxt_int = int32_t;

	enum DATA_TYPE_ : nxt_enum
	{
		DATA_TYPE_BYTE = 0x1400, // Byte
		DATA_TYPE_UBYTE = 0x1401, // Unsigned Byte
		DATA_TYPE_SHORT = 0x1402, // Short
		DATA_TYPE_USHORT = 0x1403, // Unsigned Short
		DATA_TYPE_INT = 0x1404, // Integer
		DATA_TYPE_UINT = 0x1405, // Unsigned Integer
		DATA_TYPE_FLOAT = 0x1406, // Float
	};

}

