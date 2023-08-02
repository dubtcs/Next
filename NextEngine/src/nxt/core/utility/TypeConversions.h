#pragma once

#include <nxt/EngineCore.h>

#include <string>

namespace nxt
{

	std::string ToString(wchar_t* charArray);

	std::wstring ToWideString(const std::string& str);

}
