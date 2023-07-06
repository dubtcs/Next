#pragma once

#include "../nxtid.h"

#include <vector>

namespace nxt::cmp
{
	struct GraphData
	{
		nxtid ID;
		nxtid Parent;
		std::vector<nxtid> Children;
		GraphData(const nxtid& parent) : ID{ GenerateID() }, Parent{parent} {}
		GraphData(const nxtid& parent, const nxtid& id) : ID{ id }, Parent{ parent } {}
		GraphData() = default;
	};
}

