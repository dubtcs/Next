#pragma once

#include <memory>
#include <nxt/EngineCore.h>

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

	enum nxtInterpolation : nxt_enum
	{
		nxtInterpolation_Linear = BIT(1),
		nxtInterpolation_Exponential = BIT(2)
	};

	enum nxtDataFormat : nxt_enum
	{
		nxtDataFormat_Scalar = BIT(1),
		nxtDataFormat_Vec2 = BIT(2),
		nxtDataFormat_Vec3 = BIT(3),
		nxtDataFormat_Vec4 = BIT(4)
	};

}

