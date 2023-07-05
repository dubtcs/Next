#pragma once

#include <chrono>

namespace nxt
{

	namespace clock
	{
		
		using time_point = std::chrono::steady_clock::time_point;

		// Get the current time
		time_point GetTime();

		// Get the duration between two time points
		float GetDuration(const time_point& start, const time_point& stop);

		// Get total app run time
		float GetRunTime();

	}

}
