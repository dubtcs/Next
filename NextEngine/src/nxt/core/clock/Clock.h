#pragma once

#include <chrono>

namespace nxt
{

	namespace clock
	{
		
		using time_point = std::chrono::steady_clock::time_point;

		time_point GetTime();
		float GetDuration(const time_point& start, const time_point& stop);

	}

}
