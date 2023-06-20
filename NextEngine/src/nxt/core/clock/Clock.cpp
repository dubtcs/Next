
#include "Clock.h"

#include <chrono>

namespace nxt::clock
{

	time_point GetTime()
	{
		return std::chrono::steady_clock::now();
	}

	double GetDuration(const time_point& start, const time_point& stop)
	{
		return std::chrono::duration<double>(stop - start).count();
	}

}
