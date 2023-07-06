#pragma once

#include <necs/include.h>

#include "../nxtid.h"

namespace nxt::cmp
{
	struct Heirarchy
	{
		nxtid ID;
		necs::Entity Parent;
		std::vector<necs::Entity> Children;
		Heirarchy(necs::Entity parent) : Parent{ parent }, ID{ necs::nil } {}
		Heirarchy() = default;
	};
}
