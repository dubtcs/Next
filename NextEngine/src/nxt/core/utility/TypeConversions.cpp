
#include "TypeConversions.h"

namespace nxt
{

	std::string WCharPtrToString(wchar_t* ptr)
	{
		// +1 for null terminator
		size_t ptrSize{ wcslen(ptr) + 1 };
		size_t charConversions{ 0 };

		// 2 Bytes per wchar
		const size_t stdCharSize{ ptrSize * 2 };

		char* charPtr{ new char[stdCharSize] };
		
		wcstombs_s(&charConversions, charPtr, stdCharSize, ptr, _TRUNCATE);

		std::string rString{ charPtr };
		delete[] charPtr;

		return rString;
	}

}
