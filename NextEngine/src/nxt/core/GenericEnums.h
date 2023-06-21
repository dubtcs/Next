#pragma once

#include <memory>

namespace nxt
{

	enum DATATYPE_ : uint32_t
	{
		DATATYPE_BYTE = 0x1400, // Byte
		DATATYPE_UBYTE = 0x1401, // Unsigned Byte
		DATATYPE_SHORT = 0x1402, // Short
		DATATYPE_USHORT = 0x1403, // Unsigned Short
		DATATYPE_INT = 0x1404, // Integer
		DATATYPE_UINT = 0x1405, // Unsigned Integer
		DATATYPE_FLOAT = 0x1406, // Float
	};

}

