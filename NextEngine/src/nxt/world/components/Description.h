#pragma once

#include <string>

namespace nxt::cmp
{
	struct Description
	{
		std::string Name;
		Description(const std::string& name) : Name{ name } {}
		Description() = default;
	};
}
