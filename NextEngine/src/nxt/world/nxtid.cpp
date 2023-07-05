
#include "nxtid.h"

#include <random>

namespace nxt
{

	static std::random_device gRandomDevice;
	static std::mt19937_64 gMtEngine{ gRandomDevice() };
	static std::uniform_int_distribution<nxtid> gDistribution;

	nxtid GenerateID()
	{
		return gDistribution(gMtEngine);
	}

}
