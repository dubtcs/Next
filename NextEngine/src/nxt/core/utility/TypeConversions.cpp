
#include "TypeConversions.h"

namespace nxt
{

	std::string ToString(wchar_t* ptr)
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

	std::wstring ToWideString(const std::string& str)
	{
		size_t ptrSize{ strlen(str.c_str()) + 1 };
		size_t converted{ 0 };
		wchar_t* wChar{ new wchar_t[ptrSize] };
		mbstowcs_s(&converted, wChar, ptrSize, str.c_str(), _TRUNCATE);
		std::wstring rStr{ wChar };
		delete[] wChar;
		return rStr;
	}

}
