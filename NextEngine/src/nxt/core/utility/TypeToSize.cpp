
#include "TypeToSize.h"

namespace nxt
{

	size_t SizeInBytes(nxt_enum dt)
	{
		switch (dt)
		{
			case(nxtDataType_Byte):
				return 1;
			case(nxtDataType_UByte):
				return 1;
			case(nxtDataType_Short):
				return 2;
			case(nxtDataType_UShort):
				return 2;
			case(nxtDataType_Int):
				return 4;
			case(nxtDataType_UInt):
				return 4;
			case(nxtDataType_Float):
				return 4;
		}
		return 0;
	}

	size_t SizeInBits(nxt_enum dt)
	{
		return SizeInBytes(dt) * 8;
	}

}
