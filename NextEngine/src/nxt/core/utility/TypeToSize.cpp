
#include "TypeToSize.h"

namespace nxt
{

	size_t GetSizeOf(nxt_enum dt)
	{
		switch (dt)
		{
			case(nxtDataType_Byte) :
				return 8;
			case(nxtDataType_UByte):
				return 8;
			case(nxtDataType_Short):
				return 16;
			case(nxtDataType_UShort):
				return 16;
			case(nxtDataType_Int):
				return 32;
			case(nxtDataType_UInt):
				return 32;
			case(nxtDataType_Float):
				return 32;
		}
		return 0;
	}

}
